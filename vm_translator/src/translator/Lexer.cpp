#include "Lexer.h"
#include <cpptrace/cpptrace.hpp>

vector<Token> Lexer::lex(const string &text) {
    auto res = std::vector<Token>();
    for (int i = 0; i < text.length(); i++) {
        auto c = text[i];
        switch (c) {
            case '\n':
                res.emplace_back(EOL);
                continue;
            case '/':
                if (i + 1 < text.length() && text[i + 1] == '/') {
                    i++;
                    while (i + 1 < text.length() && text[i + 1] != '\n') {
                        i++;
                    }
                    continue;
                }
                throw cpptrace::logic_error("Should this be a comment: " + text);
            case ' ':
                continue;
        }
        if (isalpha(c)) {
            string s{c};
            while (i + 1 < text.length() && isalpha(text[i + 1])) {
                s += text[i + 1];
                i++;
            }
            i++;
            if (tokenTypeMap.contains(s)) {
                res.emplace_back(tokenTypeMap.at(s));
            } else {
                throw cpptrace::logic_error("Unknown keyword: " + s);
            }
        } else if (isdigit(c)) {
            string s{c};
            while (i + 1 < text.length() && isdigit(text[i + 1])) {
                s += text[i + 1];
                i++;
            }
            res.emplace_back(Number, stoi(s));
        } else {
            throw cpptrace::logic_error("Unknown character: " + c);
        }
    }
loop_end:
    res.emplace_back(Eof);
    return res;
}
