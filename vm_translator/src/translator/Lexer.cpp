#include "Lexer.h"
#include <cpptrace/cpptrace.hpp>

#include "exception.h"

vector<Token> Lexer::lex(const string &text) {
    auto res = std::vector<Token>();
    auto len = text.length();
    for (int i = 0, line_number = 0; i < len; i++) {
        auto c = text[i];
        if (c == '\n') {
            res.emplace_back(EOL);
            line_number++;
        } else if (c == '/') {
            if (i + 1 < text.length() && text[i + 1] == '/') {
                i++;
                while (i + 1 < text.length() && text[i + 1] != '\n') {
                    i++;
                }
            }else {
                throw InvalidToken(line_number, "Should this be a comment: " + text);
            }
        } else if (isalpha(c)) {
            string s{c};
            while (i + 1 < text.length() && isalpha(text[i + 1])) {
                s += text[i + 1];
                i++;
            }
            if (tokenTypeMap.contains(s)) {
                res.emplace_back(tokenTypeMap.at(s));
            } else {
                throw InvalidToken(line_number, "Unknown keyword: " + s);
            }
        } else if (isdigit(c)) {
            string s{c};
            while (i + 1 < text.length() && isdigit(text[i + 1])) {
                s += text[i + 1];
                i++;
            }
            res.emplace_back(Number, stoi(s));
        } else if (c != ' ') {
            throw InvalidToken(line_number, "Unknown character: " + c);
        }
    }
    res.emplace_back(Eof);
    return res;
}
