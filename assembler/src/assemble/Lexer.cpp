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

void Lexer::check_overflow(const std::string &text, int value) {
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
}

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
            case '@': {
                res.emplace_back(At, i);
                string symbol;
                bool hasOnlyDigits = true;
                int startPos = i + 1;
                while (i + 1 < text.length() && text[i + 1] != '\n' && text[i + 1] != '/') {
                    /**
                     *A symbol may be any sequence of alphabetic (upper and lower case) or numeric digits.
                     *Symbols may also contain any of the following characters: under bar (“_”),
                     *period(“.”), dollar sign (“$”), and colon (“:”).
                     *Symbols may not begin with a digit character.
                     *
                     */
                    if (isalpha(text[i + 1]) || text[i + 1] == '_' || text[i + 1] == '.' || text[i + 1] == '$' ||
                        text[i + 1] == ':') {
                        symbol += text[i + 1];
                        hasOnlyDigits = false;
                    } else if (isdigit(text[i + 1])) {
                        symbol += text[i + 1];
                    } else {
                        throw invalid_symbol_exception("Invalid symbol on line " + text);
                    }
                    i++;
                }
                if (symbol.empty()) {
                    throw invalid_a_instruction_exception("Invalid A instruction on line " + text);
                }
                if (hasOnlyDigits) {
                    auto value = std::stoi(symbol);
                    check_overflow(text, value);
                    res.emplace_back(Number, startPos, value);
                } else {
                    if (isdigit(symbol.at(0))) {
                        throw invalid_symbol_exception("Symbol cannot start with digit on line " + text);
                    }
                    res.emplace_back(Symbol, startPos, 0, symbol);
                }
                break;
            }
            case ';': {
                std::string jump;
                while (i + 1 < text.length() && text[i + 1] != '\n' && text[i + 1] != ' ') {
                    jump += text[i + 1];
                    i++;
                }
                if (!jump_map.contains(jump)) {
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
                if (!res.empty() && res.back().type == Minus && (
                        res.size() < 2 || res[res.size() - 2].category != Identifier)) {
                    res.pop_back();
                    number = "-";
                    start_pos--;
                }
                for (; i < text.length() && isdigit(text[i]); i++) {
                    number += text[i];
                }
                i--;
                auto value = std::stoi(number);
                check_overflow(text, value);

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
