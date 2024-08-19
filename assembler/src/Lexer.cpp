#include "Lexer.h"

#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <cpptrace/cpptrace.hpp>
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


inline constexpr const char *ToString(TokenType v) {
    switch (v) {
        case Plus:
            return "Plus";
        case Minus:
            return "Minus";
        case Assignment:
            return "Assignment";
        case And:
            return "And";
        case Or:
            return "Or";
        case Not:
            return "Not";
        case D:
            return "D";
        case A:
            return "A";
        case M:
            return "M";
        case Number:
            return "Constant";
        case NegativeOne:
            return "NegativeOne";
        case Zero:
            return "Zero";
        case One:
            return "One";
        case JGT:
            return "JGT";
        case JGE:
            return "JGE";
        case JEQ:
            return "JEQ";
        case JNE:
            return "JNE";
        case JLT:
            return "JLT";
        case JLE:
            return "JLE";
        case JMP:
            return "JMP";
        case At:
            return "At";
        case EOL:
            return "EOL";
        case Eof:
            return "Eof";
        default:
            return "Unknown";
    }
}

std::vector<Token> Lexer::lex(const std::string &text) {
    auto res = std::vector<Token>();
    for (int i = 0; i < text.length(); i++) {
        auto c = text[i];
        switch (c) {
            case '\n':
                res.emplace_back(EOL, i);
            case ' ':
                break;
            case '/':
                if (i + 1 < text.length() && text[i + 1] == '/') {
                    for (; i + 1 < text.length() && text[i + 1] != '\n'; i++);
                    break;
                }
                throw cpptrace::logic_error("Should this be a  comment: " + text);
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
                if(value == -1){
                    res.emplace_back(NegativeOne, start_pos);
                } else if (value == 0) {
                    res.emplace_back(Zero, start_pos);
                } else if (value == 1) {
                    res.emplace_back(One, start_pos);
                } else {
                    res.emplace_back(Number, start_pos, value);
                }
                break;
            }
            default:
                throw cpptrace::logic_error("Unknown symbol: " + std::string(1, c) + " on line " + text);
        }
    }
    res.emplace_back(Eof, text.length());

    return res;
}
