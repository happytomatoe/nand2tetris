#include "Translator.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <regex>

#include "Lexer.h"
#include "Validator.h"
#include "Token.h"
#include "MemorySegment.h"
using namespace token;

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


string Translator::handle_push(const string &file_name,
                               map<memory::MemorySegmentPointer, int> &pointerToAddress, const int line_number,
                               vector<Token>::const_iterator &it) {
    string res;
    constexpr TokenType operation = Push;
    switch ((++it)->type) {
        case Local:
        case Argument:
        case This:
        case That: {
            //push from memory segment onto stack
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            res += format("//push {} {} \n", toString(memorySementTokenType), number);
            auto memorySegmentPointer = memory::getSegmentPointer(memorySementTokenType);
            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
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
            if (++pointerToAddress.at(memory::StackPointer) > memory::memorySegmentMinMaxAdress.at(
                    memory::StackPointer).max) {
                throw PointerOutOfRangeException(
                    "Stack pointer is out of range on line " + to_string(line_number));
            }
            break;
        }
        case ConstantMemorySegment: {
            // SP=i; SP++
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            check_overflow(number);
            res += format("//push {} {} \n", toString(memorySementTokenType), number);
            res += format(R"(
                                                                @{}
                                                                D=A
                                                          )", number);
            res += stackPush();
            if (++pointerToAddress.at(memory::StackPointer) > memory::memorySegmentMinMaxAdress.at(
                    memory::StackPointer).max) {
                throw PointerOutOfRangeException(
                    "Stack pointer is out of range on line " + to_string(line_number));
            }

            break;
        }
        case StaticMemorySegment: {
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            //TODO: should we do an overflow check?

            res += operationComment(operation, memorySementTokenType, number);
            // @<file-name>.i
            // stack.push(M)
            string file_name_without_extension = file_name.substr(0, file_name.find_last_of('.'));
            res += format(R"(
                                @{}.{}
                                D=M
                            )", file_name_without_extension, number);
            res += stackPush();
            if (++pointerToAddress.at(memory::StackPointer) > memory::memorySegmentMinMaxAdress.at(
                    memory::StackPointer).max) {
                throw PointerOutOfRangeException(
                    "Stack pointer is out of range on line " + to_string(line_number));
            }
            break;
        }
        case Temp: {
            //push from memory segment onto stack
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            res += operationComment(operation, memorySementTokenType, number);
            auto memorySegmentPointer = memory::getSegmentPointer(memorySementTokenType);
            auto memSegmentRange = getMemorySegmentMinMaxAdress(memorySegmentPointer);

            if (number < 0 || number > memSegmentRange.max - memSegmentRange.min) {
                throw InvalidOperation(
                    "Invalid operation on line " + to_string(line_number) +
                    ".Temp number must be between 0 and 7");
            }
            //addr=symbolAddress+i; *SP=*addr; SP++;
            res += format(R"(
                                @{}
                                D=M
                            )", number + memSegmentRange.min);
            res += stackPush();
            if (++pointerToAddress.at(memory::StackPointer) > memory::memorySegmentMinMaxAdress.at(
                    memory::StackPointer).max) {
                throw PointerOutOfRangeException(
                    "Stack pointer is out of range on line " + to_string(line_number));
            }
            break;
        }
        case Pointer: {
            /**
                                push pointer 0/1 *SP=THIS/THAT; SP++
                                0=this address
                                1=that address
                            */
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            res += operationComment(operation, memorySementTokenType, number);

            if (number != 0 && number != 1) {
                throw PointerOutOfRangeException("Pointer number can be 0 or 1");
            }
            auto symbolAdress = number == 1
                                    ? memory::getSymbolAdress(memory::getSegmentPointer(This))
                                    : memory::getSymbolAdress(memory::getSegmentPointer(That));
            res += format(R"(
                                 @{}
                                 A=M
                                 D=M
                            )", symbolAdress);
            res += stackPush();
            if (++pointerToAddress.at(memory::StackPointer) > memory::memorySegmentMinMaxAdress.at(
                    memory::StackPointer).max) {
                throw PointerOutOfRangeException(
                    "Stack pointer is out of range on line " + to_string(line_number));
            }
            break;
        }
        default:
            throw InvalidOperation("Invalid operaiton on line " + to_string(line_number));
    }
    return res;
}

string Translator::handle_pop(map<memory::MemorySegmentPointer, int> &pointerToAddress, const int line_number,
                              vector<Token>::const_iterator &it) {
    string res;
    const TokenType operation = Pop;
    switch ((++it)->type) {
        case Local:
        case Argument:
        case This:
        case That: {
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            auto memorySegmentPointer = memory::getSegmentPointer(memorySementTokenType);
            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
            if (number < 0) {
                throw InvalidOperation("Invalid operation on line " + to_string(line_number));
            }
            res += operationComment(operation, memorySementTokenType, number);

            // pop mem_segment1 i      addr=LCL+i; SP--; *addr=*SP
            //TODO is it better to calculate LCL+i and use result in assembly?
            res += format(R"(
                @{}
                D=A
                @{}
                D=D+A
                @pop_mem_temp
                M=D
            )", symbolAdress, number);
            res += stackPop();
            if (--pointerToAddress.at(memory::StackPointer) < memory::memorySegmentMinMaxAdress.at(
                    memory::StackPointer).min) {
                throw PointerOutOfRangeException(
                    "Stack pointer is out of range on line " + to_string(line_number));
            }
            res += R"(
                @pop_mem_temp
                M=D
            )";
            break;
        }
        case StaticMemorySegment: {
            break;
        }
        case Temp: {
            break;
        }
        case Pointer: {
            break;
        }
        default:
            throw InvalidOperation("Invalid operaiton on line " + to_string(line_number));
    }
    return res;
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
                auto operation = it->type;
                if (operation == Push) {
                    res += handle_push(file_name, pointerToAddress, line_number, it);
                    break;
                } else if (operation == Pop) {
                    res += handle_pop(pointerToAddress, line_number, it);
                    break;
                } else {
                    throw InvalidOperation("Invalid operaiton on line " + to_string(line_number));
                }
                break;
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

string Translator::stackPop() {
    auto spSymbolAdress = memory::getSymbolAdress(memory::StackPointer);
    return format(R"(
                                //D=*SP;
                                @{}
                                A=M
                                D=M
                                //SP--
                                @{}
                                M=M-1
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

string Translator::operationComment(TokenType operation, TokenType memorySementTokenType, int number) {
    return format("//{} {} {} \n", toString(operation), toString(memorySementTokenType), number);
}
