#include "Lexer.h"

#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <cpptrace/cpptrace.hpp>

#include "exception.h"
using namespace std;

const map<std::string, TokenType> jump_map = {
    {"JGT", JGT},
    {"JGE", JGE},
    {"JEQ", JEQ},
    {"JNE", JNE},
    {"JLT", JLT},
    {"JLE", JLE},
    {"JMP", JMP},
};

std::vector<Token> Lexer::lex(const std::string &text) {
    auto res = std::vector<Token>();
    for (int i = 0; i < text.length(); i++) {
        auto c = text[i];


        switch (c) {
            case '\n':
                res.emplace_back(EOL, i);
                break;
            case '/':
                if (i + 1 < text.length() && text[i + 1] == '/') {
                    goto loop_end;
                }
                throw cpptrace::logic_error("Should this be a  comment: " + text);
            case ' ':
                break;
            //operators
            case '+':
                res.emplace_back(Plus, i);
                break;
            case '-':
                res.emplace_back(Minus, i);
                break;
            case '=':
                res.emplace_back(Assignment, i);
                break;
            case '&':
                res.emplace_back(And, i);
                break;
            case '|':
                res.emplace_back(Or, i);
                break;
            case '!':
                res.emplace_back(Not, i);
                break;
            case '@':
                res.emplace_back(At, i);
                break;
            case ';': {
                std::string jump;
                while (i + 1 < text.length() && text[i + 1] != '\n') {
                    jump += text[i + 1];
                    i++;
                }
                if (jump_map.find(jump) == jump_map.end()) {
                    throw cpptrace::logic_error("Unknown jump: " + jump);
                }
                res.emplace_back(jump_map.at(jump), i);
                break;
            }
            case 'D':
                res.emplace_back(D, i);
                break;
            case 'A':
                res.emplace_back(A, i);
                break;
            case 'M':
                res.emplace_back(M, i);
                break;
            case '9':
            case '8':
            case '7':
            case '6':
            case '5':
            case '4':
            case '3':
            case '2':
            case '1':
            case '0': {
                std::string number;
                auto start_pos = i;
                if (res.back().type == Minus) {
                    res.pop_back();
                    number = "-";
                    start_pos--;
                }
                for (; i < text.length() && isdigit(text[i]); i++) {
                    number += text[i];
                }
                i--;
                auto value = std::stoi(number);
                if (value < numeric_limits<signed short>::min()) {
                    throw number_overflow_exception(
                        "Number overflow on line " + text + " \nMin value is " + std::to_string(
                            numeric_limits<signed short>::min()));
                }
                if (value > numeric_limits<signed short>::max()) {
                    throw number_overflow_exception(
                        "Number overflow on line " + text + " \nMax value is " + std::to_string(
                            numeric_limits<signed short>::max()));
                }

                if (value == -1) {
                    res.emplace_back(NegativeOne, start_pos);
                } else if (value == 0) {
                    res.emplace_back(Zero, start_pos);
                } else if (value == 1) {
                    res.emplace_back(One, start_pos);
                } else {
                    res.emplace_back(Number, start_pos, value, "");
                }
                break;
            }
            default:
                throw cpptrace::logic_error("Unknown symbol: " + std::string(1, c) + " on line " + text);
        }
    }
loop_end:
    res.emplace_back(Eof, text.length());

    return res;
}
