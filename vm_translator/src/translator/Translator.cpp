#include "Translator.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <regex>

#include "Lexer.h"
#include "Validator.h"
#include "Token.h"
#include "MemorySegment.cpp"

string Translator::translate(const string &file_path) {
    ifstream file(file_path);
    if (!file.good()) {
        throw runtime_error("Failed to open file: " + file_path);
    }
    file.close();
    string line, text;

    while (file.good() && getline(file, line)) {
        text += line + "\n";
    }
    auto tokens = Lexer::lex(text);
    Validator::checkOrder(tokens);
    string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);
    return translate(tokens, base_filename);
}


inline map<memory::MemorySegmentPointer, int> initMemorySegments() {
    map<memory::MemorySegmentPointer, int> pointerToAddress;
    for (auto [k,v]: memory::memorySegmentMinMaxAdress) {
        pointerToAddress[k] = v.min;
    }
    return pointerToAddress;
}

const map<TokenType, memory::MemorySegmentPointer> tokenTypeToMemorySegmentPointer = {
    {Local, memory::Local},
    {Argument, memory::Arg},
    {This, memory::This},
    {That, memory::That},
    {Pointer, memory::Pointer},
    {Temp, memory::Temp},
};

memory::MemorySegmentPointer getSegmentPointer(TokenType t) {
    if (!tokenTypeToMemorySegmentPointer.contains(t)) {
        throw cpptrace::logic_error("No corresponding segment pointer for token type " +
                                    toString(t));
    }
    return tokenTypeToMemorySegmentPointer.at(t);
}

string Translator::translate(const vector<Token> &tokens, const string &file_name) {
    string res;
    //init memory segments
    map<memory::MemorySegmentPointer, int> pointerToAddress = initMemorySegments();

    res += "//memory init\n";
    for (auto [p,address]: memory::symbolAdress) {
        res += format(R"(
            @{}
            D = A
            @{}
            M = D
        )", pointerToAddress.at(p), address);
    }
    res += "//memory init end\n\n";
    int line_number = 0;
    const auto end = tokens.end();
    for (auto it = tokens.begin(); it != end; ++it, line_number++) {
        switch (it->category) {
            case Terminal:
                continue;
            // 2 types of operations
            //move operation+memory segment+ number
            case MoveOperation: {
                //Should be translated to
                if (it->type == Push) {
                    switch ((++it)->type) {
                        case Local:
                        case Argument:
                        case This:
                        case That: {
                            //push from memory segment onto stack
                            res += "//push " + toString(it->type);
                            auto memorySegmentPointer = getSegmentPointer(it->type);
                            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
                            auto number = (++it)->number;
                            res += " " + to_string(number) + "\n";
                            if (number < 0) {
                                throw InvalidOperation("Invalid operation on line " + to_string(line_number));
                            }
                            //addr=symbolAddress+i; *SP=*addr; SP++;
                            res += format(R"(
                                //addr=symbolAddress+i;
                                @{}
                                D=M
                                @{}
                                D=D+A
                                A=D
                                D=M

                            )", symbolAdress, number);
                            res += stackPush();
                            if (++pointerToAddress.at(memory::StackPointer) >= memory::memorySegmentMinMaxAdress.at(
                                    memory::StackPointer).max) {
                                throw PointerOutOfRangeException(
                                    "Stack pointer out of range on line " + to_string(line_number));
                            }
                            break;
                        }
                        case ConstantMemorySegment: {
                            // SP=i; SP++
                            auto number = (++it)->number;
                            string s;
                            check_overflow(number);
                            auto symbolAdress = memory::symbolAdress.at(memory::StackPointer);
                            res += "//push constant " + to_string(number) + "\n";
                            string command = format(R"(
                                                                @{}
                                                                D=A
                                                                @{}
                                                                A=M
                                                                M=D
                                                                @{}
                                                                M=M+1
                                                          )", number, symbolAdress, symbolAdress);

                            res += command;
                            if (++pointerToAddress.at(memory::StackPointer) >= memory::memorySegmentMinMaxAdress.at(
                                    memory::StackPointer).max) {
                                throw PointerOutOfRangeException(
                                    "Stack pointer out of range on line " + to_string(line_number));
                            }

                            break;
                        }
                        case StaticMemorySegment: {
                            string file_name_without_extension = file_name.substr(0, file_name.find_last_of('.'));
                            auto number = (++it)->number;
                            res += "//push static " + to_string(number) + "\n";
                            // @<file-name>.i
                            // stack.push(M)
                            res += format(R"(
                                @{}.{}
                                D=M
                            )", file_name_without_extension, number);
                            res += stackPush();
                            if (++pointerToAddress.at(memory::StackPointer) >= memory::memorySegmentMinMaxAdress.at(
                                    memory::StackPointer).max) {
                                throw PointerOutOfRangeException(
                                    "Stack pointer out of range on line " + to_string(line_number));
                            }
                            break;
                        }
                        case Temp: {
                            //push from memory segment onto stack
                            res += "//push " + toString(it->type);
                            auto memorySegmentPointer = getSegmentPointer(it->type);
                            auto memSegmentRange = memory::getMemorySegmentMinMaxAdress(memorySegmentPointer);

                            auto spSymbolAdress = memory::getSymbolAdress(memory::StackPointer);
                            auto number = (++it)->number;
                            res += " " + to_string(number) + "\n";
                            if (number < 0 || number > memSegmentRange.max - memSegmentRange.min) {
                                throw InvalidOperation(
                                    "Invalid operation on line " + to_string(line_number) +
                                    ".Temp number must be between 0 and 7");
                            }
                            //addr=symbolAddress+i; *SP=*addr; SP++;
                            res += format(R"(
                                @{}
                                D=M
                            )", number + memSegmentRange.min, spSymbolAdress, spSymbolAdress);
                            res += stackPush();
                            if (++pointerToAddress.at(memory::StackPointer) >= memory::memorySegmentMinMaxAdress.at(
                                    memory::StackPointer).max) {
                                throw PointerOutOfRangeException(
                                    "Stack pointer out of range on line " + to_string(line_number));
                            }
                            break;
                        }
                        case Pointer: {
                            /**
                                push pointer 0/1 *SP=THIS/THAT; SP++
                                0=this address
                                1=that address
                            */
                            auto number = (++it)->number;
                            if (number != 0 && number != 1) {
                                throw PointerOutOfRangeException("Pointer number can be 0 or 1");
                            }
                            auto symbolAdress = number == 1
                                                    ? memory::getSymbolAdress(getSegmentPointer(This))
                                                    : memory::getSymbolAdress(getSegmentPointer(That));
                            res += format(R"(
                                 @{}
                                 A=M
                                 D=M
                            )", symbolAdress);
                            res += stackPush();
                            if (++pointerToAddress.at(memory::StackPointer) >= memory::memorySegmentMinMaxAdress.at(
                                    memory::StackPointer).max) {
                                throw PointerOutOfRangeException(
                                    "Stack pointer out of range on line " + to_string(line_number));
                            }
                            break;
                        }
                        default:
                            throw InvalidOperation("Invalid operaiton on line " + to_string(line_number));
                    }
                    break;
                } else {
                    throw NotImplementedException();
                    break;
                }
            }
            case ArithmeticOrLogicOperation: {
                throw NotImplementedException();
            }
            default:
                throw InvalidOperation("Invalid operaiton on line " + to_string(line_number));
        }
    }
    //TODO: find if it's possible to strip margin from raw string
    return res;
}

string Translator::stackPush() {
    auto spSymbolAdress = memory::getSymbolAdress(memory::StackPointer);
    return format(R"(
                                //*SP=*addr;
                                @{}
                                A=M
                                M=D
                                //SP++
                                @{}
                                M=M+1
                            )", spSymbolAdress, spSymbolAdress);
}


void Translator::check_overflow(int value) {
    if (value < numeric_limits<signed short>::min()) {
        throw NumberOverflowException(
            "Number overflow.\nMin value is " + std::to_string(
                numeric_limits<signed short>::min()));
    }
    if (value > numeric_limits<signed short>::max()) {
        throw NumberOverflowException(
            "Number overflow.\nMax value is " + std::to_string(
                numeric_limits<signed short>::max()));
    }
}
