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
    //TODO: add other identifiers
    Number,
    //do I need this?
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


enum Category {
    AssignmentOperation,
    OtherOperation,
    AtCategory,
    PredefinedConstant, // -1, 0, 1
    Identifier,          // D, A, M
    Jump,
    Other
};

struct Token {
    Category category;
    TokenType type;
    int startPos, constValue;

    Token(TokenType type, int start_pos, int constValue = 0)
        : type(type),
          category(getCategory(type)),
          constValue(constValue),
          startPos(start_pos) {
    }

    Token(Token *other)
        : type(other->type),
          category(other->category),
          constValue(other->constValue),
          startPos(other->startPos) {
    }

    friend bool operator==(const Token &lhs, const Token &rhs) {
        return lhs.category == rhs.category
               && lhs.type == rhs.type
               && lhs.startPos == rhs.startPos
               && lhs.constValue == rhs.constValue;
    }

    friend bool operator!=(const Token &lhs, const Token &rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const Token &obj) {
        return os
               << "Token(category: " << toString(obj.category)
               << ", type: " << toString(obj.type)
               << ", startPos: " << obj.startPos
               << ", constValue: " << obj.constValue << ")";
    }

    static constexpr const char *toString(TokenType v) {
        switch (v) {
            case Plus:
                return "+";
            case Minus:
                return "-";
            case Assignment:
                return "=";
            case And:
                return "&";
            case Or:
                return "|";
            case Not:
                return "!";
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

private:
    constexpr const Category getCategory(TokenType type) {
        switch (type) {
            case At:
                return AtCategory;
            case Assignment:
                return AssignmentOperation;
            case Plus:
            case Minus:
            case And:
            case Or:
            case Not:
                return OtherOperation;
            case D:
            case A:
            case M:
                return Identifier;
            case Number:
            case NegativeOne:
            case Zero:
            case One:
                return PredefinedConstant;
            case JGT:
            case JGE:
            case JEQ:
            case JNE:
            case JLT:
            case JLE:
            case JMP:
                return Jump;
            default:
                return Other;
        }
    }

    static constexpr const char *toString(const Category v) {
        switch (v) {
            case AtCategory:
                return "AtCategory";
            case AssignmentOperation:
                return "AssignmentOperation";
            case OtherOperation:
                return "OtherOperation";
            case PredefinedConstant:
                return "PredefinedConstant";
            case Identifier:
                return "Identifier";
            case Jump:
                return "Jump";
            case Other:
                return "Other";
            default:
                return "Unknown";
        }
    }
};

class Lexer {
public:
    static std::vector<Token> lex(const std::string &text);

private:
    static const map<string, TokenType> jumpMap;
};
#endif //LEXER_H
