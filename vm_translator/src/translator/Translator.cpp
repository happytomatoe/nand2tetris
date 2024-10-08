#include "Translator.h"

#include <fstream>
#include <iostream>
#include <random>
#include <regex>

#include "Lexer.h"
#include "MemorySegment.h"
#include "StringUtils.h"
#include "Token.h"
#include "Validator.h"
#include "assembly.h"
#include "exception.h"
using namespace token;

const Config Translator::default_config =
    Config{memory::default_memory_segment_min_max_adress, false, false};

string Translator::translate(const string &file_path) {
    ifstream file(file_path);
    if (!file.good()) {
        throw cpptrace::runtime_error("Failed to open file: " + file_path);
    }
    string line, text;

    while (file.good() && getline(file, line)) {
        text += line + "\n";
    }
    file.close();
    if (text.empty()) {
        throw cpptrace::runtime_error("Empty file: " + file_path);
    }
    try {
        auto tokens = Lexer::lex(text);
        Validator::checkOrder(tokens);
        string base_filename =
            file_path.substr(file_path.find_last_of("/\\") + 1);
        auto s = translate(tokens, base_filename);
        const regex empty_lines("[\n\\s]{2,}");
        s = regex_replace(s, empty_lines, "\n");
        return s;
    } catch (BaseException &e) {
        cout << "Exception on line " << e.line_number + 1 << endl
             << getLine(text, e.line_number) << endl;
        throw;
    }
}

string Translator::translate(const vector<Token> &tokens,
                             const string &file_name) {
    stack_size_counting_enabled = has_no_goto_or_if_goto(tokens);
    string res;
    TokenType previous_operation_type;
    // init memory segments

    if (config.memory_init) {
        res += initializeMemorySegments();
    }
    for (int i = 0; i < tokens.size(); ++i, line_number++) {
        if (config.enable_comments && i < tokens.size() - 1 &&
            tokens[i + 1].category != Terminal)
            res += "|//line number " + to_string(line_number) + "\n";
        switch (tokens[i].category) {
            case Terminal:
                ++line_number;
                continue;
            // 2 types of operations
            // move operation+memory segment+ number
            case MoveOperation: {
                const auto operation = tokens[i].type;
                const auto memorySementTokenType = tokens[++i].type;
                const auto number = tokens[++i].number;
                if (operation == Push) {
                    res +=
                        handle_push(file_name, memorySementTokenType, number);
                    if (stack_size_counting_enabled) {
                        int maxStackSize =
                            get_memory_segment_min_max_address(memory::Stack)
                                .max -
                            get_memory_segment_min_max_address(memory::Stack)
                                .min +
                            1;
                        if (stack_size > maxStackSize) {
                            throw StackPointerOutOfRangeException(line_number);
                        }
                    }
                    previous_operation_type = operation;
                    break;
                } else if (operation == Pop) {
                    res += handle_pop(file_name, memorySementTokenType, number);
                    if (stack_size_counting_enabled) {
                        if (stack_size < 0) {
                            throw StackPointerOutOfRangeException(line_number);
                        }
                    }
                    previous_operation_type = operation;
                    break;
                } else {
                    throw InvalidOperation(line_number);
                }
            }
            case ArithmeticOrLogicOperation: {
                res += handle_arithmetic_logical_operation(
                    line_number, tokens[i], previous_operation_type);

                previous_operation_type = tokens[i].type;
                break;
            }
            case LabelCategory: {
                res += format("({})\n", tokens[i].label);

                previous_operation_type = tokens[i].type;
                break;
            }
            case IfGoToCategory: {
                if (config.enable_comments) {
                    res += "|//if go to";
                }
                res += stack_pop_into_d_register();
                res += format(R"(
                    |@{}
                    |D;JGT
                )",
                              tokens[i].label);

                previous_operation_type = tokens[i].type;
                break;
            }
            case GoToCategory: {
                res += format(R"(
                    |@{}
                    |0;JMP
                )",
                              tokens[i].label);

                previous_operation_type = tokens[i].type;
                break;
            }
            case FunctionCategory: {
                if (auto token = tokens[i]; token.type == Function) {
                    res += handle_function_declaration(token);
                } else if (token.type == Call) {
                    res += handle_function_call(token);
                } else if (token.type == Return) {
                    res += handle_function_return();
                } else {
                    throw InvalidToken(line_number, "Unknown operation");
                }
                previous_operation_type = tokens[i].type;
                break;
            }
            default:
                throw InvalidOperation(line_number);
        }
    }
    return translator::stripMargin(res);
}

inline bool Translator::has_no_goto_or_if_goto(const vector<Token> &tokens) {
    return ranges::find_if(tokens, [](const Token &token) {
               return token.type == Goto || token.type == IfGoto;
           }) == tokens.end();
}

string Translator::initializeMemorySegments() {
    string res;
    if (config.enable_comments) {
        res += "//memory init\n";
    }
    for (auto [p, address] : memory::symbol_adress) {
        if (config.enable_comments) {
            res += format("|//memory segment {}", toString(p));
        }
        res += format(R"(
            |@{}
            |D=A
            |@{}
            |M=D
        )",
                      config.memory_segment_min_max_adress.at(p).min, address);
    }
    if (config.enable_comments) {
        res += "|//memory init end\n";
    }
    return res;
}

string Translator::handle_function_call(const Token &token) {
    string res;

    if (token.functionArgumentCount == 0) {
        if (config.enable_comments)
            res += "|//push arg 0 when calling void function\n";
        res += "|D=0\n";
        res += push_onto_stack();
    }
    auto argCount =
        token.functionArgumentCount == 0 ? 1 : token.functionArgumentCount;
    if (config.enable_comments)
        res += "|// save arg segment to later set the new function arg segment";
    res += format(R"(
                            
                            |@{}
                            |D=A
                            |@{}
                            |D=D-M
                            |D=-D
                            |@temp_new_arg
                            |M=D
                        )",
                  argCount, getSymbolAdress(memory::Stack));
    string call_end_label = "CALL_END_" + to_string(random_int());
    res += format(R"(
                        |@{}
                        |D=A
                    )",
                  call_end_label);
    res += push_onto_stack();

    for (auto memory_segment : memory_segments_to_save_on_function_call) {
        if (config.enable_comments)
            res += format("|// save {} segment", toString(memory_segment));
        res += format(R"(
                            
                            |@{}
                            |D=M
                        )",
                      getSymbolAdress(memory_segment));
        res += push_onto_stack();
    }
    if (config.enable_comments) res += "|//set new ARG";
    res += format(R"(
                        
                        |@temp_new_arg
                        |D=M
                        |@{0}
                        |M=D
                        |@{1}
                        |D=M
                        |@{2}
                        |M=D
                    )",
                  getSymbolAdress(memory::MemorySegment::Arg),
                  getSymbolAdress(memory::MemorySegment::Stack),
                  getSymbolAdress(memory::MemorySegment::Local));
    if (config.enable_comments) res += " |//jump to function";
    res += format(R"(
                        |@{}
                        |0;JMP
                    )",
                  token.functionName);
    res += format("|({})\n", call_end_label);
    return res;
}

string Translator::handle_function_return() {
    string res;
    auto return_adress_var_name = "return_address";
    auto arg_one_var_name = "current_function_arg_1_" + to_string(random_int());
    auto clear_stack_loop_name = "clear_stack_loop_" + to_string(random_int());
    if (config.enable_comments) res += "|//copy return value to arg 0";
    res += format(R"(
                        
                        |@{}
                        |A=M-1
                        |D=M
                        |@{}
                        |A=M
                        |M=D
                    )",
                  getSymbolAdress(memory::MemorySegment::Stack),
                  getSymbolAdress(memory::MemorySegment::Arg));
    if (config.enable_comments) res += "|//save arg 1 into var";
    res +=
        format(R"(
                        |@{}
                        |D=M+1
                        |@{}
                        |M=D
                    )",
               getSymbolAdress(memory::MemorySegment::Arg), arg_one_var_name);
    if (config.enable_comments) res += "|//save return address";
    res += format(R"(
                        |@5
                        |D=A
                        |@{}
                        |D=D-M
                        |A=-D
                        |D=M
                        |@{}
                        |M=D
                    )",
                  getSymbolAdress(memory::MemorySegment::Local),
                  return_adress_var_name);
    auto saved_local_var_name = "saved_local";
    if (config.enable_comments) res += " |//save local into var";
    res += format(R"(
                        |@{}
                        |D=M
                        |@{}
                        |M=D
                    )",
                  getSymbolAdress(memory::MemorySegment::Local),
                  saved_local_var_name);
    for (auto memorySegment : memory_segments_to_restore_on_function_return) {
        if (config.enable_comments)
            res +=
                format("|//recover memory segment {}", toString(memorySegment));
        res += format(R"(
                        |@{}
                        |MD=M-1
                        |A=D
                        |D=M
                        |@{}
                        |M=D
                    )",
                      saved_local_var_name, getSymbolAdress(memorySegment));
    }
    if (config.clear_stack) {
        if (config.enable_comments) res += "|//clear the stack";
        res += format(R"(
                        |@{0}
                        |D=M-1
                        |@{1}
                        |D=D-M
                        |({2})
                        |@{1}
                        |A=D+M
                        |M=0
                        |D=D-1
                        |@{2}
                        |D;JGE
                    )",
                      getSymbolAdress(memory::MemorySegment::Stack),
                      arg_one_var_name, clear_stack_loop_name);
    }
    if (config.enable_comments)
        res += "|//set stack pointer to current function arg 1";
    res +=
        format(R"(
                        |@{}
                        |D=M
                        |@{}
                        |M=D
                    )",
               arg_one_var_name, getSymbolAdress(memory::MemorySegment::Stack));
    if (config.enable_comments) res += "|//jump to return address";
    res += format(R"(
                        |@{}
                        |A=M
                        |0;JMP
                    )",
                  return_adress_var_name,
                  getSymbolAdress(memory::MemorySegment::Local));
    return res;
}

string Translator::handle_function_declaration(const Token &token) {
    string res;
    res += format(R"(
                    |({})
                )",
                  token.functionName);
    if (token.functionArgumentCount > 0) {
        if (config.enable_comments) res += "|//set local pointer";
        res += format(R"(
                        |@{}
                        |D=M
                        |@{}
                        |M=D
                    )",
                      getSymbolAdress(memory::Stack),
                      getSymbolAdress(memory::Local));
        if (config.enable_comments) res += "|//set local vars\n";
        res += "|D=0\n";
        for (int j = 0; j < token.functionArgumentCount; ++j) {
            res += push_onto_stack();
        }
    }
    return res;
}

inline memory::Range Translator::get_memory_segment_min_max_address(
    const memory::MemorySegment &p) const {
    if (!config.memory_segment_min_max_adress.contains(p)) {
        throw cpptrace::invalid_argument("Invalid memory segment pointer");
    }
    return config.memory_segment_min_max_adress.at(p);
}

string Translator::getLine(const string &text, const int line_number) {
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

string Translator::handle_arithmetic_logical_operation(
    const int line_number, const Token &token, const TokenType &previous_operation) {
    string res;
    switch (const auto operation = token.type) {
        case Add: {
            res += operationComment(operation);
            res +=
                two_operand_operation(operation, "M=D+M", previous_operation);
            break;
        }
        case Subtract: {
            res += operationComment(operation);
            res += two_operand_operation(operation, R"(
                |D=D-M
                |M=-D
            )",
                                         previous_operation);
            break;
        }
        case Negate: {
            if (stack_size_counting_enabled) {
                if (stack_size < 1) {
                    throw InvalidOperation(line_number,
                                           format("Operation on empty stack",
                                                  toString(operation)));
                }
            }

            res += operationComment(operation);
            res += format(R"(
                           |@{}
                           |A=M-1
                           |M=-M
                        )",
                          getSymbolAdress(memory::Stack));
            break;
        }
        case GreaterThan:
        case LessThan:
        case Equals: {
            res += operationComment(operation);
            res += logicalComparison(operation);
            break;
        }
        case And: {
            res += operationComment(operation);
            res +=
                two_operand_operation(operation, "M=D&M", previous_operation);
            break;
        }
        case Or: {
            res += operationComment(operation);
            res +=
                two_operand_operation(operation, "M=D|M", previous_operation);
            break;
        }
        case Not: {
            if (stack_size_counting_enabled) {
                if (stack_size < 1) {
                    throw InvalidOperation(line_number,
                                           format("Operation on empty stack",
                                                  toString(operation)));
                }
            }
            res += operationComment(operation);
            res += format(R"(
                            |@{}
                            |A=M-1
                            |M=!M
                        )",
                          getSymbolAdress(memory::Stack));
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}

/**
 *
 * @param operation - add, subtract,...
 * @param operation_instructions - assembly instructions
 * @param clear_stack - if true freed memory will be set to 0
 */
string Translator::two_operand_operation(const TokenType &operation,
                                         const string &operation_instructions,
                                         const TokenType &previous_token_type) {
    if (stack_size_counting_enabled) {
        if (stack_size < 2) {
            throw InvalidOperation(
                line_number, format("Not enough values to do operation {}",
                                    toString(operation)));
        }
        stack_size--;
    }
    string res;
    if (previous_token_type != Push) {
        res = format(R"(
            |@{}
            |M=M-1
            |A=M
            |D=M{}
        )",
                     getSymbolAdress(memory::Stack),
                     config.clear_stack ? "\nM=0" : "");
    }
    return res + format(R"(
       |A=A-1
       |{}
    )",
                        operation_instructions);
}

string Translator::file_name_without_extension(const string &file_name) {
    return file_name.substr(0, file_name.find_last_of('.'));
}

string Translator::handle_push(const string &file_name,
                               const TokenType memorySementTokenType,
                               int number) {
    string res;
    constexpr TokenType operation = Push;
    switch (memorySementTokenType) {
        case Local:
        case Argument:
        case This:
        case That: {
            res += operationComment(operation, memorySementTokenType, number);
            const auto memorySegmentPointer =
                memory::getMemorySegment(memorySementTokenType);
            auto symbolAdress = getSymbolAdress(memorySegmentPointer);
            checkAdressOutOfRange(
                number, memory::getMemorySegment(memorySementTokenType));
            res += format(R"(
                                |@{}
                                |D=M
                                |@{}
                                |D=D+A
                                |A=D
                                |D=M
                            )",
                          symbolAdress, number);
            res += push_onto_stack();
            break;
        }
        case Constant: {
            // SP=i; SP++
            check_overflow(number);
            res += operationComment(operation, memorySementTokenType, number);
            if (number >= -1 & number <= 1) {
                res += push_onto_stack(to_string(number));
            } else {
                res += format(R"(
                            |@{}
                            |D=A
                      )",
                              number);
                res += push_onto_stack();
            }
            break;
        }
        case Static: {
            checkAdressOutOfRange(
                number, memory::getMemorySegment(memorySementTokenType));

            res += operationComment(operation, memorySementTokenType, number);
            // @<file-name>.i
            // stack.push(M)
            res += format(R"(
                                |@{}.{}
                                |D=M
                            )",
                          file_name_without_extension(file_name), number);
            res += push_onto_stack();
            break;
        }
        case Temp: {
            // push from memory segment onto stack
            res += operationComment(operation, memorySementTokenType, number);
            const auto memorySegmentPointer =
                memory::getMemorySegment(memorySementTokenType);
            const auto memSegmentRange =
                get_memory_segment_min_max_address(memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentPointer);
            // addr=symbolAddress+i; *SP=*addr; SP++;
            res += format(R"(
                                |@{}
                                |D=M
                            )",
                          number + memSegmentRange.min);
            res += push_onto_stack();
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
                throw PointerOutOfRangeException(
                    line_number, "Pointer number can be 0 or 1");
            }
            auto symbolAdress =
                number == 0 ? getSymbolAdress(memory::getMemorySegment(This))
                            : getSymbolAdress(memory::getMemorySegment(That));
            res += format(R"(
                                 |@{}
                                 |D=M
                            )",
                          symbolAdress);
            res += push_onto_stack();
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}

string Translator::handle_pop(const string &file_name,
                              const TokenType memorySementTokenType,
                              int number) {
    string res;
    constexpr TokenType operation = Pop;
    switch (memorySementTokenType) {
        case Local:
        case Argument:
        case This:
        case That: {
            const auto memorySegmentPointer =
                memory::getMemorySegment(memorySementTokenType);
            auto symbolAdress = getSymbolAdress(memorySegmentPointer);
            checkAdressOutOfRange(number, memorySegmentPointer);
            res += operationComment(operation, memorySementTokenType, number);

            // pop mem_segment1 i      addr=LCL+i; SP--; *addr=*SP
            res += format(R"(
                |@{}
                |D=M
                |@{}
                |D=D+A
                |@pop_normal_segment_temp
                |M=D
            )",
                          symbolAdress, number);
            res += stack_pop_into_d_register();
            res += R"(
                |@pop_normal_segment_temp
                |A=M
                |M=D
            )";
            break;
        }
        case Static: {
            checkAdressOutOfRange(
                number, memory::getMemorySegment(memorySementTokenType));
            /**
                *- pop static i
                D=stack.pop
                @<file-name>.i
                M=D
                */
            res += operationComment(operation, memorySementTokenType, number);
            res += stack_pop_into_d_register();
            res += format(R"(
                |@{}.{}
                |M=D
            )",
                          file_name_without_extension(file_name), number);
            break;
        }
        case Temp: {
            const auto memorySegmentPointer =
                memory::getMemorySegment(memorySementTokenType);
            const auto memSegmentRange =
                get_memory_segment_min_max_address(memorySegmentPointer);
            checkAdressOutOfRange(
                number, memory::getMemorySegment(memorySementTokenType));
            res += operationComment(operation, memorySementTokenType, number);
            // pop temp i      addr=5+i; SP--; *addr=*SP
            res += stack_pop_into_d_register();
            res += format(R"(
                |@{}
                |M=D
            )",
                          memSegmentRange.min + number);

            break;
        }
        case Pointer: {
            res += operationComment(operation, memorySementTokenType, number);
            if (number != 0 && number != 1) {
                throw PointerOutOfRangeException(
                    line_number, "Pointer number can be 0 or 1");
            }
            auto symbolAdress =
                number == 0 ? getSymbolAdress(memory::getMemorySegment(This))
                            : getSymbolAdress(memory::getMemorySegment(That));
            /*
                   pop pointer 0/1		SP--; THIS/THAT=*SP
                   0=this address
                   1=that address */
            res += stack_pop_into_d_register();
            res += format(R"(
               |@{}
               |M=D
            )",
                          symbolAdress);
            break;
        }
        default:
            throw InvalidOperation(line_number);
    }
    return res;
}

string Translator::push_onto_stack(const string &value_to_push) {
    if (stack_size_counting_enabled) {
        stack_size++;
    }
    auto spSymbolAdress = getSymbolAdress(memory::Stack);
    string res;
    if (config.enable_comments) res += "|//stack push";
    return res + format(R"(
                                |@{}
                                |AM=M+1
                                |A=A-1
                                |M={}
                            )",
                        spSymbolAdress, value_to_push);
}

string Translator::stack_pop_into_d_register() {
    if (stack_size_counting_enabled) {
        if (stack_size < 1) {
            throw InvalidOperation(line_number, "Can't do pop empty stack");
        }
        stack_size--;
    }
    string res;
    if (config.enable_comments) res += "|//stack pop";
    return res + format(R"(
                                
                                |@{0}
                                |M=M-1
                                |A=M
                                |D=M{1}
                            )",
                        getSymbolAdress(memory::Stack),
                        config.clear_stack ? "\nM=0" : "");
}

string Translator::logicalComparison(const TokenType &type) {
    if (stack_size_counting_enabled) {
        if (stack_size < 2) {
            throw InvalidOperation(
                line_number,
                format("Not enough values to do operation {}", toString(type)));
        }
        stack_size--;
    }
    auto spSymbolAdress = getSymbolAdress(memory::Stack);
    const auto jumpType = assembly::tokenTypeToJumpType(type);
    const auto cmpEq = format("D;{}", to_string(jumpType));
    const auto cmpOther = format(R"(-D;{})", to_string(jumpType));
    string operation = jumpType == assembly::Jump::JEQ ? cmpEq : cmpOther;
    // TODO: find out why the web ide and nand2tetris program returns -1 when
    // logical comparison is true
    // TODO: the reason why tests fail
    return format(R"(
               |@{0}
               |M=M-1
               |A=M
               |D=M{3}
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
            )",
                  spSymbolAdress, random_int(), operation,
                  config.clear_stack ? "\n|M=0" : "");
}

void Translator::checkAdressOutOfRange(const int &value,
                                       const memory::MemorySegment &p) const {
    const auto min_max_address = get_memory_segment_min_max_address(p);

    const auto memorySegmentMaxStackSize =
        min_max_address.max - min_max_address.min;

    if (value < 0) {
        throw AdressOutOfMemorySegmentRange(line_number, min_max_address);
    }
    if (value > memorySegmentMaxStackSize) {
        throw AdressOutOfMemorySegmentRange(line_number, min_max_address);
    }
}

void Translator::check_overflow(const int number) const {
    if (number < numeric_limits<signed short>::min()) {
        throw NumberOverflowException(
            line_number,
            "Number overflow.\nMin value is " +
                std::to_string(numeric_limits<signed short>::min()));
    }
    if (number > numeric_limits<signed short>::max()) {
        throw NumberOverflowException(
            line_number,
            "Number overflow.\nMax value is " +
                std::to_string(numeric_limits<signed short>::max()));
    }
}

inline string Translator::operationComment(
    const TokenType operation, const TokenType memorySementTokenType,
    int number) {
    string res;
    if (config.enable_comments)
        res += format("\n|//{} {} {} \n", toString(operation),
                      toString(memorySementTokenType), number);
    return res;
}

inline string Translator::operationComment(const TokenType operation) {
    string res;
    if (config.enable_comments)
        res += format("\n|//{} \n", toString(operation));
    return res;
}
