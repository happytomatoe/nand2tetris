#include "Translator.h"

#include <fstream>
#include <iostream>
#include <random>
#include <regex>

#include "assembly.h"
#include "Lexer.h"
#include "Validator.h"
#include "Token.h"
#include "MemorySegment.h"
#include "StringUtils.h"
using namespace token;


const string Translator::program_end = translator::stripMargin(R"(
        |(END)
        |    @END
        |    0;JMP
    )");

string Translator::translate(const string &file_path) {
    ifstream file(file_path);
    if (!file.good()) {
        throw runtime_error("Failed to open file: " + file_path);
    }
    string line, text;

    while (file.good() && getline(file, line)) {
        text += line + "\n";
    }
    file.close();
    if (text.empty()) {
        throw runtime_error("Empty file: " + file_path);
    }
    try {
        auto tokens = Lexer::lex(text);
        Validator::checkOrder(tokens);
        string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);
        return translate(tokens, base_filename);
    } catch (BaseException &e) {
        cout << "Exception on line " << e.line_number + 1 << endl << getLine(text, e.line_number) << endl;
        throw;
    }
}

string Translator::initializeMemorySegments() {
    string res;
    res += "//memory init\n";
    for (auto [p,address]: memory::symbolAdress) {
        res += format(R"(
            |@{}
            |D=A
            |@{}
            |M=D
        )", memory::memorySegmentMinMaxAdress.at(p).min, address);
    }
    res += "|//memory init end\n";
    return res;
}

string Translator::translate(const vector<Token> &tokens, const string &file_name) {
    string res;
    //init memory segments
    int stackSize = 0;
    res += initializeMemorySegments();
    int line_number = 0;
    const auto end = tokens.end();
    for (auto it = tokens.begin(); it != end; ++it, line_number++) {
        res += "|//line number " + to_string(line_number) + "\n";
        switch (it->category) {
            case Terminal:
                continue;
            // 2 types of operations
            //move operation+memory segment+ number
            case MoveOperation: {
                auto operation = it->type;
                if (operation == Push) {
                    res += handle_push(file_name, stackSize, line_number, it);
                    int maxStackSize = memory::getMemorySegmentMinMaxAdress(memory::Stack).max -
                                       memory::getMemorySegmentMinMaxAdress(memory::Stack).min + 1;
                    if (stackSize > maxStackSize) {
                        throw StackPointerOutOfRangeException(line_number);
                    }
                    break;
                } else if (operation == Pop) {
                    res += handle_pop(file_name, stackSize, line_number, it);
                    if (stackSize < 0) {
                        throw StackPointerOutOfRangeException(line_number);
                    }
                    break;
                } else {
                    throw InvalidOperation(line_number);
                }
            }
            case ArithmeticOrLogicOperation: {
                res += handle_arithmetic_logical_operation(stackSize, line_number, it);
                break;
            }
            default:
                throw InvalidOperation(line_number);
        }
    }
    //program end
    res += program_end;
    return translator::stripMargin(res);
}


string Translator::getLine(const string &text, int line_number) {
    for (int i = 0, l = 0; i < text.length(); ++i) {
        if (text[i] == '\n') {
            l++;
        }
        if (l == line_number) {
            if (text[i] == '\n') i++;
            string res;
            while (i < text.length() && text[i] != '\n') {
                res += text[i];
                i++;
            }
            return res;
        }
    }
    cerr << "line with number " << line_number << " not found" << endl;
    return "";
}

string Translator::handle_arithmetic_logical_operation(int &stackSize,
                                                       int line_number, vector<Token>::const_iterator it) {
    string res;
    switch (auto operation = it->type) {
        case Add: {
            res += operationComment(operation);
            res += two_operand_operation("M=D+M");
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }

            stackSize--;
            break;
        }
        case Subtract: {
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }
            res += operationComment(operation);
            res += two_operand_operation(R"(
                |D=D-M
                |M=-D
            )");
            stackSize--;
            break;
        }
        case Negate: {
            if (stackSize < 1) {
                throw InvalidOperation(line_number, format(
                                           "Operation on empty stack",
                                           toString(operation)));
            }

            res += operationComment(operation);
            res += format(R"(
                           |@{}
                           |AM=M-1
                           |M=-M
                        )", memory::getSymbolAdress(memory::Stack));
            break;
        }
        case GreaterThan:
        case LessThan:
        case Equals: {
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }
            res += operationComment(operation);
            res += logicalComparison(operation);
            stackSize--;
            break;
        }
        case And: {
            res += operationComment(operation);
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }
            res += operationComment(operation);
            res += two_operand_operation("M=D&M");
            stackSize--;
            break;
        }
        case Or: {
            res += operationComment(operation);
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }
            res += two_operand_operation("M=D|M");
            stackSize--;
            break;
        }
        case Not: {
            if (stackSize < 1) {
                throw InvalidOperation(line_number, format(
                                           "Operation on empty stack",
                                           toString(operation)));
            }
            res += operationComment(operation);
            res += format(R"(
                            |@{}
                            |A=M-1
                            |M=!M
                        )", memory::getSymbolAdress(memory::Stack));
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}

/**
 *
 * @param operation - like +,-,&,|. Check assembly guide for details
 */
string Translator::two_operand_operation(string operation) {
    auto spSymbolAdress = memory::getSymbolAdress(memory::Stack);
    return format(R"(
       |@{}
       |M=M-1
       |A=M
       |D=M
       |A=A-1
       |{}
    )", spSymbolAdress, operation);
}


string Translator::file_name_without_extension(const string &file_name) {
    return file_name.substr(0, file_name.find_last_of('.'));
}

string Translator::handle_push(const string &file_name,
                               int &stackSize, const int line_number,
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
            res += operationComment(operation, memorySementTokenType, number);
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memory::getMemorySegment(memorySementTokenType), line_number);

            //addr=symbolAddress+i; *SP=*addr; SP++;
            res += format(R"(
                                |@{}
                                |D=M
                                |@{}
                                |D=D+A
                                |A=D
                                |D=M
                            )", symbolAdress, number);
            res += stackPush();
            stackSize++;
            break;
        }
        case Constant: {
            // SP=i; SP++
            auto memorySegmentTokenType = it->type;
            auto number = (++it)->number;
            check_overflow(number, line_number);
            res += operationComment(operation, memorySegmentTokenType, number);
            res += format(R"(
                            |@{}
                            |D=A
                      )", number);
            res += stackPush();
            stackSize++;
            break;
        }
        case Static: {
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            checkAdressOutOfRange(number, memory::getMemorySegment(memorySementTokenType), line_number);

            res += operationComment(operation, memorySementTokenType, number);
            // @<file-name>.i
            // stack.push(M)
            res += format(R"(
                                |@{}.{}
                                |D=M
                            )", file_name_without_extension(file_name), number);
            res += stackPush();
            stackSize++;
            break;
        }
        case Temp: {
            //push from memory segment onto stack
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;


            res += operationComment(operation, memorySementTokenType, number);
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto memSegmentRange = getMemorySegmentMinMaxAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentPointer, line_number);
            //addr=symbolAddress+i; *SP=*addr; SP++;
            res += format(R"(
                                |@{}
                                |D=M
                            )", number + memSegmentRange.min);
            res += stackPush();
            stackSize++;
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
                throw PointerOutOfRangeException(line_number, "Pointer number can be 0 or 1");
            }
            auto symbolAdress = number == 0
                                    ? memory::getSymbolAdress(memory::getMemorySegment(This))
                                    : memory::getSymbolAdress(memory::getMemorySegment(That));
            res += format(R"(
                                 |@{}
                                 |D=M
                            )", symbolAdress);
            res += stackPush();
            stackSize++;
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}

string Translator::handle_pop(const string &file_name,
                              int &stackSize,
                              const int line_number,
                              vector<Token>::const_iterator &it) {
    string res;
    constexpr TokenType operation = Pop;
    switch ((++it)->type) {
        case Local:
        case Argument:
        case This:
        case That: {
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentPointer, line_number);
            res += operationComment(operation, memorySementTokenType, number);

            // pop mem_segment1 i      addr=LCL+i; SP--; *addr=*SP
            res += format(R"(
                |@{}
                |D=M
                |@{}
                |D=D+A
                |@pop_normal_segment_temp
                |M=D
            )", symbolAdress, number);
            res += stackPop();
            stackSize--;
            res += R"(
                |@pop_normal_segment_temp
                |A=M
                |M=D
            )";
            break;
        }
        case Static: {
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            checkAdressOutOfRange(number, memory::getMemorySegment(memorySementTokenType), line_number);
            /**
            *- pop static i
            D=stack.pop
            @<file-name>.i
            M=D
            */
            res += operationComment(operation, memorySementTokenType, number);
            res += stackPop();
            stackSize--;
            res += format(R"(
                |@{}.{}
                |M=D
            )", file_name_without_extension(file_name), number);
            break;
        }
        case Temp: {
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto memSegmentRange = getMemorySegmentMinMaxAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memory::getMemorySegment(memorySementTokenType), line_number);
            res += operationComment(operation, memorySementTokenType, number);
            //pop temp i      addr=5+i; SP--; *addr=*SP
            res += stackPop();
            res += format(R"(
                |@{}
                |M=D
            )", memSegmentRange.min + number);

            break;
        }
        case Pointer: {
            //TODO: add check if number is is memory range. Probably not the used one?
            auto memorySementTokenType = it->type;
            auto number = (++it)->number;
            res += operationComment(operation, memorySementTokenType, number);

            if (number != 0 && number != 1) {
                throw PointerOutOfRangeException(line_number, "Pointer number can be 0 or 1");
            }
            auto symbolAdress = number == 0
                                    ? memory::getSymbolAdress(memory::getMemorySegment(This))
                                    : memory::getSymbolAdress(memory::getMemorySegment(That));
            /*
               pop pointer 0/1		SP--; THIS/THAT=*SP
               0=this address
               1=that address */
            res += stackPop();
            stackSize--;
            res += format(R"(
               |@{}
               |M=D
            )", symbolAdress);
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}


string Translator::stackPush() {
    auto spSymbolAdress = memory::getSymbolAdress(memory::Stack);
    return format(R"(
                                |//stack push
                                |@{}
                                |A=M
                                |M=D
                                |@{}
                                |M=M+1
                            )", spSymbolAdress, spSymbolAdress);
}

string Translator::stackPop() {
    auto spSymbolAdress = memory::getSymbolAdress(memory::Stack);
    return format(R"(
                                |//stack pop;
                                |@{}
                                |M=M-1
                                |A=M
                                |D=M
                            )", spSymbolAdress, spSymbolAdress, spSymbolAdress);
}

inline int Translator::intRand(const int &min, const int &max) {
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

string Translator::logicalComparison(TokenType type) {
    int imax = std::numeric_limits<int>::max();
    // int randNumber = intRand(imin, imax);
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(0, imax);
    int randNumber = distribution(generator);
    auto spSymbolAdress = getSymbolAdress(memory::Stack);
    auto jumpType = assembly::tokenTypeToJumpType(type);
    auto cmpEq = format("D;{}", toString(jumpType));
    auto cmpOther = format(R"(-D;{})", toString(jumpType));
    string operation = jumpType == assembly::Jump::JEQ ? cmpEq : cmpOther;
    //TODO: find or implement templatign
    return format(R"(
               |@{0}
               |M=M-1
               |A=M
               |D=M
               |A=A-1
               |D=D-M
               |@IF_TRUE_{1}
               |{2}
               |(IF_FALSE_{1})
               |     @{0}
               |     A=M-1
               |     M=0
               |     @END_CHECK_{1}
               |     0;JMP
               |(IF_TRUE_{1})
               |     @{0}
               |     A=M-1
               |     M=-1
               |(END_CHECK_{1})
            )", spSymbolAdress, randNumber, operation);
}


void Translator::checkAdressOutOfRange(const int &value, const memory::MemorySegment &p, const int &line_number) {
    if (value < 0) {
        throw AdressOutOfMemorySegmentRange(line_number);
    }
    auto memorySegmentMaxStackSize = memory::getMemorySegmentMinMaxAdress(p).max -
                                     memory::getMemorySegmentMinMaxAdress(p).min;
    if (value > memorySegmentMaxStackSize) {
        throw AdressOutOfMemorySegmentRange(line_number);
    }
}

void Translator::check_overflow(int value, int line_number) {
    if (value < numeric_limits<signed short>::min()) {
        throw NumberOverflowException(line_number,
                                      "Number overflow.\nMin value is " + std::to_string(
                                          numeric_limits<signed short>::min()));
    }
    if (value > numeric_limits<signed short>::max()) {
        throw NumberOverflowException(line_number,
                                      "Number overflow.\nMax value is " + std::to_string(
                                          numeric_limits<signed short>::max()));
    }
}


string Translator::operationComment(TokenType operation, TokenType memorySementTokenType, int number) {
    return format("\n|//{} {} {} \n", toString(operation), toString(memorySementTokenType), number);
}

string Translator::operationComment(TokenType operation) {
    return format("\n|//{} \n", toString(operation));
}


inline map<memory::MemorySegment, int> Translator::initMemorySegments() {
    map<memory::MemorySegment, int> pointerToAddress;
    for (auto [k,v]: memory::memorySegmentMinMaxAdress) {
        pointerToAddress[k] = v.min;
    }
    return pointerToAddress;
}
