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

string Translator::translate(const string &file_path,
                             const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMax) {
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
        return translate(tokens, base_filename, memorySegmentsMinMax);
    } catch (BaseException &e) {
        cout << "Exception on line " << e.line_number + 1 << endl << getLine(text, e.line_number) << endl;
        throw;
    }
}


string Translator::initializeMemorySegments(
    const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAdress) {
    string res;
    res += "//memory init\n";
    for (auto [p,address]: memory::symbolAdress) {
        res += format(R"(
            |@{}
            |D=A
            |@{}
            |M=D
        )", memorySegmentsMinMaxAdress.at(p).min, address);
    }
    res += "|//memory init end\n";
    return res;
}

string Translator::translate(const vector<Token> &tokens, const string &file_name,
                             const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMax) {
    string res;
    res += initializeMemorySegments(memorySegmentsMinMax);
    for (int i = 0; i < tokens.size(); ++i, line_number++) {
        res += "|//line number " + to_string(line_number) + "\n";
        switch (tokens[i].category) {
            case Terminal:
                continue;
            // 2 types of operations
            //move operation+memory segment+ number
            case MoveOperation: {
                auto operation = tokens[i].type;
                auto memorySementTokenType = tokens[++i].type;
                auto number = tokens[++i].number;
                if (operation == Push) {
                    res += handle_push(file_name, line_number, memorySementTokenType, number, memorySegmentsMinMax);
                    int maxStackSize = get(memorySegmentsMinMax, memory::Stack).max -
                                       get(memorySegmentsMinMax, memory::Stack).min + 1;
                    if (stackSize > maxStackSize) {
                        throw StackPointerOutOfRangeException(line_number);
                    }
                    break;
                } else if (operation == Pop) {
                    res += handle_pop(file_name, line_number, memorySementTokenType, number,
                                      memorySegmentsMinMax);
                    if (stackSize < 0) {
                        throw StackPointerOutOfRangeException(line_number);
                    }
                    break;
                } else {
                    throw InvalidOperation(line_number);
                }
            }
            case ArithmeticOrLogicOperation: {
                res += handle_arithmetic_logical_operation(line_number, tokens[i]);
                break;
            }
            case LabelCategory: {
                res += format("({})\n", tokens[i].label);
                break;
            }
            case IfGoToCategory: {
                //cond = pop
                //if cond jump to
                res += stackPop();
                res += format(R"(
                    |@{}
                    |D;JGT
                )", tokens[i].label);
                break;
            }
            case GoToCategory: {
                //cond = pop
                res += stackPop();
                res += format(R"(
                    |@{}
                    |0;JMP
                )", tokens[i].label);
                break;
            }
            case FunctionCategory: {
                auto token = tokens[i];
                if (token.type == Function) {
                    res += format(R"(
                    |({})
                )", token.functionName);
                    //set local pointer
                    res += format(R"(
                        |@{}
                        |D=M
                        |@{}
                        |M=D
                    )", memory::getSymbolAdress(memory::Stack), memory::getSymbolAdress(memory::Local));
                    //set local args
                    if (token.functionArgumentCount > 0) {
                        res += "|D=0\n";
                        for (int i = 0; i < token.functionArgumentCount; ++i) {
                            res += stackPush();
                        }
                    }
                } else if (token.type == Call) {
                    //set arg
                    // auto arg = sp-nargs
                    //TODO: add return address
                    //return address is a label. How to save its' address in the memory?
                    //save place for return address
                    res += "D=-1\n";
                    res += stackPush();
                    res += format(R"(
                            |// save arg segment
                            |@{}
                            |D=A
                            |@{}
                            |D=D-M
                            |D=-D
                            @temp_new_arg
                            M=D
                        )", token.functionArgumentCount, memory::getSymbolAdress(memory::Stack));
                    auto returnAddress = line_number + 1;

                    for (auto memorySegment: {
                             memory::MemorySegment::Local, memory::MemorySegment::Arg,
                             memory::MemorySegment::This, memory::MemorySegment::That
                         }) {
                        res += format(R"(
                            // save {} segment
                            |@{}
                            |D=M
                        )", toString(memorySegment), memory::getSymbolAdress(memorySegment));
                        res += stackPush();
                    }
                    res += format(R"(
                        |//set new ARG
                        |@temp_new_arg
                        |D=M
                        |{0}
                        |M=D
                        |@{1}
                        |D=M
                        |@{2}
                        |M=D
                    )", memory::getSymbolAdress(memory::MemorySegment::Arg),
                                  memory::getSymbolAdress(memory::MemorySegment::Stack),
                                  memory::getSymbolAdress(memory::MemorySegment::Local));
                } else if (token.type == Return) {
                    //copy return value to arg 0
                    // handle_pop()
                    throw NotImplementedException();

                } else {
                    throw InvalidToken(line_number, "Unknown operation");
                }
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

inline memory::Range Translator::get(
    const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress, const memory::MemorySegment &p) {
    if (!memorySegmentsMinMaxAddress.contains(p)) {
        throw cpptrace::invalid_argument("Invalid memory segment pointer");
    }
    return memorySegmentsMinMaxAddress.at(p);
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

string Translator::handle_arithmetic_logical_operation(const int line_number, const Token &token) {
    string res;
    switch (auto operation = token.type) {
        case Add: {
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }
            res += operationComment(operation);
            res += two_operand_operation("M=D+M");


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
                           |A=M-1
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
            break;
        }
        case And: {
            if (stackSize < 2) {
                throw InvalidOperation(line_number, format(
                                           "Not enough values to do operation {}",
                                           toString(operation)));
            }
            res += operationComment(operation);
            res += two_operand_operation("M=D&M");
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
    stackSize--;
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

string Translator::handle_push(const string &file_name, const int line_number,
                               TokenType memorySementTokenType, int number,
                               const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress
) {
    string res;
    constexpr TokenType operation = Push;
    switch (memorySementTokenType) {
        case Local:
        case Argument:
        case This:
        case That: {
            //push from memory segment onto stack

            res += operationComment(operation, memorySementTokenType, number);
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentsMinMaxAddress,
                                  memory::getMemorySegment(memorySementTokenType),
                                  line_number);

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
            break;
        }
        case Constant: {
            // SP=i; SP++
            check_overflow(number, line_number);
            res += operationComment(operation, memorySementTokenType, number);
            res += format(R"(
                            |@{}
                            |D=A
                      )", number);
            res += stackPush();
            break;
        }
        case Static: {
            checkAdressOutOfRange(number, memorySegmentsMinMaxAddress,
                                  memory::getMemorySegment(memorySementTokenType),
                                  line_number);

            res += operationComment(operation, memorySementTokenType, number);
            // @<file-name>.i
            // stack.push(M)
            res += format(R"(
                                |@{}.{}
                                |D=M
                            )", file_name_without_extension(file_name), number);
            res += stackPush();
            break;
        }
        case Temp: {
            //push from memory segment onto stack
            res += operationComment(operation, memorySementTokenType, number);
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto memSegmentRange = get(memorySegmentsMinMaxAddress, memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentsMinMaxAddress, memorySegmentPointer, line_number);
            //addr=symbolAddress+i; *SP=*addr; SP++;
            res += format(R"(
                                |@{}
                                |D=M
                            )", number + memSegmentRange.min);
            res += stackPush();
            break;
        }
        case Pointer: {
            /**
                                push pointer 0/1 *SP=THIS/THAT; SP++
                                0=this address
                                1=that address
                            */
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
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}

string Translator::handle_pop(const string &file_name,
                              const int line_number,
                              TokenType memorySementTokenType, int number,
                              const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress) {
    string res;
    constexpr TokenType operation = Pop;
    switch (memorySementTokenType) {
        case Local:
        case Argument:
        case This:
        case That: {
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto symbolAdress = memory::getSymbolAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentsMinMaxAddress, memorySegmentPointer, line_number);
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
            res += R"(
                |@pop_normal_segment_temp
                |A=M
                |M=D
            )";
            break;
        }
        case Static: {
            checkAdressOutOfRange(number, memorySegmentsMinMaxAddress,
                                  memory::getMemorySegment(memorySementTokenType),
                                  line_number);
            /**
            *- pop static i
            D=stack.pop
            @<file-name>.i
            M=D
            */
            res += operationComment(operation, memorySementTokenType, number);
            res += stackPop();
            res += format(R"(
                |@{}.{}
                |M=D
            )", file_name_without_extension(file_name), number);
            break;
        }
        case Temp: {
            auto memorySegmentPointer = memory::getMemorySegment(memorySementTokenType);
            auto memSegmentRange = get(memorySegmentsMinMaxAddress, memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentsMinMaxAddress,
                                  memory::getMemorySegment(memorySementTokenType),
                                  line_number);
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
    stackSize++;
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
    stackSize--;
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
    //TODO: find out why the web ide and nand2tetris program returns -1 when logical comparison is true
    stackSize--;
    //TODO: the reason why tests fail
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
               |     M=1
               |(END_CHECK_{1})
            )", spSymbolAdress, randNumber, operation);
}


void Translator::checkAdressOutOfRange(const int &value,
                                       const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMax,
                                       const memory::MemorySegment &p, const int &line_number) {
    if (value < 0) {
        throw AdressOutOfMemorySegmentRange(line_number);
    }
    auto memorySegmentMaxStackSize = get(memorySegmentsMinMax, p).max -
                                     get(memorySegmentsMinMax, p).min;
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
