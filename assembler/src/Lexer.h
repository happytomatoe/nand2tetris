//
// Created by babkamen on 8/18/24.
//

#ifndef LEXER_H
#define LEXER_H
#include <map>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// enum OperatorType {
//     Plus,
//     Minus,
//     Assignment,
//     And,
//     Or,
//     Not
// };

// enum Operands {
//     Identifier,
//     Constant
// };

// enum Identifiers {
//     D,
//     A,
//     M,
//     OtherIndetifier
// };


enum TokenType {
    //operators
    Plus,
    Minus,
    Assignment,
    And,
    Or,
    Not,
    //identifiers
    D,
    A,
    M,
    OtherIndetifier,
    //constants
    Constant,
    NegativeOne,
    Zero,
    One,
    JGT,
    JGE,
    JEQ,
    JNE,
    JLT,
    JLE,
    JMP,
    //other
    At, //@
    EOL,
    Eof,
};


enum JumpType {
};

inline const char *ToString(TokenType v) {
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
        case OtherIndetifier:
            return "OtherIndetifier";
        case Constant:
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

struct Token {
    TokenType type;
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
private:
    static const map<string,TokenType> jumpMap;
};
#endif //LEXER_H
