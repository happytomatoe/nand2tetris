#include "Lexer.h"
#include <set>
#include <cpptrace/cpptrace.hpp>

#include "exception.h"
set empty_space_chars = {' ', '\t', '\r'};

inline bool isKeywordOrIdentifierStart(const char &c) {
    return isalpha(c) || c == '-';
}


inline bool isKeywordOrIdentifier(const char &c) {
    /**
       *A symbol may be any sequence of alphabetic (upper and lower case) or numeric digits.
       *Symbols may also contain any of the following characters: under bar (“_”),
       *period(“.”), dollar sign (“$”), and colon (“:”).
       *Symbols may not begin with a digit character.
       *
       */
    return isalpha(c) || c == '_' || c == '-' || c == '.' || c == '$' || c == ':' || isdigit(c);
}

vector<Token> Lexer::lex(const string &inputFile) {
    auto res = std::vector<Token>();
    const auto len = inputFile.length();
    for (int i = 0, line_number = 0; i < len; i++) {
        auto c = inputFile[i];
        if (c == '\n') {
            res.emplace_back(EOL);
            line_number++;
        } else if (c == '/') {
            if (i + 1 < inputFile.length() && inputFile[i + 1] == '/') {
                i++;
                while (i + 1 < inputFile.length() && inputFile[i + 1] != '\n') {
                    i++;
                }
            } else {
                throw InvalidToken(line_number, "Should this be a comment: " + inputFile);
            }
        } else if (isKeywordOrIdentifierStart(c)) {
            /**
               *A symbol may be any sequence of alphabetic (upper and lower case) or numeric digits.
               *Symbols may also contain any of the following characters: under bar (“_”),
               *period(“.”), dollar sign (“$”), and colon (“:”).
               *Symbols may not begin with a digit character.
               *
               */
            string s{c};
            while (i + 1 < inputFile.length() && isKeywordOrIdentifier(inputFile[i + 1])) {
                s += inputFile[i + 1];
                i++;
            }
            if (tokenTypeMap.contains(s)) {
                res.emplace_back(tokenTypeMap.at(s));
            } else {
                const auto last = &res.back();
                if (last->type == Label || last->type == IfGoto || last->type == Goto) {
                    last->label = s;
                } else if (last->type == Function || last->type == Call) {
                    last->functionName = s;
                } else {
                    throw InvalidToken(line_number, format("Unexpected token: '{}'", s));
                }
            }
        } else if (isdigit(c)) {
            string s{c};
            while (i + 1 < inputFile.length() && isdigit(inputFile[i + 1])) {
                s += inputFile[i + 1];
                i++;
            }
            int number = stoi(s);
            const auto last = &res.back();
            if (last->type == Function || last->type == Call) {
                last->functionArgumentCount = number;
            } else {
                res.emplace_back(Number, number);
            }
        } else if (!empty_space_chars.contains(c)) {
            throw InvalidToken(line_number, format("Unknown character: '{}'", c));
        }
    }
    res.emplace_back(Eof);
    return res;
}
