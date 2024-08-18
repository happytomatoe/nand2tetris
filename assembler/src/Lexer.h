//
// Created by babkamen on 8/18/24.
//

#ifndef LEXER_H
#define LEXER_H
#include <ostream>
#include <string>
#include <vector>

enum Type {
    //operators
    Plus,
    Minus,
    Assignment,
    And,
    Or,
    Not,
    //keywords
    Identifier,
    D,
    A,
    M,
    Jump,
    Constant,
    MinusOne,
    Zero,
    One,
    At, //@
    EOL,
    Eof,
};

inline const char *ToString(Type v) {
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
        case Identifier:
            return "Identifier";
        case D:
            return "D";
        case A:
            return "A";
        case M:
            return "M";
        case Jump:
            return "Jump";
        case Constant:
            return "Constant";
        case MinusOne:
            return "MinusOne";
        case Zero:
            return "Zero";
        case One:
            return "One";
        case At:
            return "At";
        case EOL:
            return "EOL";
        case Eof:
            return "EOF";
        default:
            return "Unknown";
    }
}

struct Token {
    Type type;
    std::string text;
    int startPos;

    friend bool operator==(const Token &lhs, const Token &rhs) {
        return lhs.type == rhs.type
               && lhs.text == rhs.text
               && lhs.startPos == rhs.startPos;
    }

    friend bool operator!=(const Token &lhs, const Token &rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const Token &obj) {
        return os
               << "Token(type:" << ToString(obj.type)
               << ", text:" << obj.text
               << ", startPos:" << obj.startPos << ")";
    }
};


class Lexer {
public:
    static std::vector<Token> lex(std::string text);
};
#endif //LEXER_H
