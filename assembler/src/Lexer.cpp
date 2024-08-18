#include "Lexer.h"

#include <stdexcept>
#include <string>
#include <vector>

std::vector<Token> Lexer::lex(std::string text) {
    auto res = std::vector<Token>();
    for (int i = 0; i < text.length(); i++) {
        auto c = text[i];
        switch (c) {
            case '\n':
                res.push_back(Token(EOL, "", i));
            case ' ':
                break;
            case '/':
                if (i + 1 < text.length() && text[i + 1] == '/') {
                    for (; i + 1 < text.length() && text[i + 1] != '\n'; i++);
                    break;
                }
                throw std::invalid_argument("Should this be a  comment: " + text);
            //operators
            case '+':
                res.push_back(Token(Plus, std::string(1, c), i));
                break;
            case '-':
                res.push_back(Token(Minus, std::string(1, c), i));
                break;
            case '=':
                res.push_back(Token(Assignment, std::string(1, c), i));
                break;

            case '&':
                res.push_back(Token(And, std::string(1, c), i));
                break;
            case '|':
                res.push_back(Token(Or, std::string(1, c), i));
                break;
            case '!':
                res.push_back(Token(Not, std::string(1, c), i));
                break;
            case '@':
                res.push_back(Token(At, std::string(1, c), i));
                break;
            case ';': {
                //TODO: validate jump?
                std::string jump = "";
                while (i+1 < text.length() && text[i+1] != '\n') {
                    jump += text[i+1];
                    i++;
                }
                res.push_back(Token(Jump, jump, i));
                break;
            }
            case 'D':
                res.push_back(Token(D, std::string(1, c), i));
                break;
            case 'A':
                res.push_back(Token(A, std::string(1, c), i));
                break;
            case 'M':
                res.push_back(Token(M, std::string(1, c), i));
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
                for (;i < text.length() && isdigit(text[i]);i++) {
                    number += text[i];
                }
                i--;
                res.push_back(Token(Constant, number, start_pos));
                break;
            }
            default:
                throw std::invalid_argument("Unknown symbol: " + std::string(1, c) + " on line " + text);
        }
    }
    res.push_back(Token(Eof, "", text.length()));

    return res;
}
