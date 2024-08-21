//
// Created by babkamen on 8/18/24.
//

#ifndef LEXER_H
#define LEXER_H
#include <map>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
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
    //TODO: add other identifiers like R1, R2, R3
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
    Symbol,
    EOL,
    Eof,
};


enum Category {
    AssignmentOperation,
    OtherOperation,
    AtCategory,
    PredefinedConstant, // -1, 0, 1
    NumberCategory,
    Identifier, // D, A, M
    Jump,
    End,
    SymbolCategory,
    Other
};

struct Token {
    Category category;
    TokenType type;
    int startPos, constValue;
    string text;

    Token(TokenType type, int start_pos, int constValue = 0, string text = "")
        : type(type),
          category(getCategory(type)),
          constValue(constValue),
          text(std::move(text)),
          startPos(start_pos) {
    }


    Token(Token *other)
        : type(other->type),
          category(other->category),
          constValue(other->constValue),
          startPos(other->startPos), text(other->text) {
    }

    friend bool operator==(const Token &lhs, const Token &rhs) {
        return lhs.category == rhs.category
               && lhs.type == rhs.type
               && lhs.startPos == rhs.startPos
               && lhs.constValue == rhs.constValue
               && lhs.text == rhs.text;
    }

    friend bool operator!=(const Token &lhs, const Token &rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const Token &obj) {
        return os
               << "Token(category: " << toString(obj.category)
               << ", type: " << toString(obj.type)
               << ", startPos: " << obj.startPos
               << ", constValue: " << obj.constValue
               << ", text: " << obj.text << ")";
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
                return "-1";
            case Zero:
                return "0";
            case One:
                return "1";
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
            case Symbol:
                return "Symbol";
            default:
                return "Unknown";
        }
    }

private:
    static constexpr const Category getCategory(TokenType type) {
        switch (type) {
            case EOL:
                return End;
            case Eof:
                return End;
            case At:
                return AtCategory;
            case Symbol:
                return SymbolCategory;
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
                return NumberCategory;
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
            case NumberCategory:
                return "NumberCategory";
            case SymbolCategory:
                return "SymbolCategory";
            case End:
                return "End";
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
    static void check_overflow(const std::string &text, int value);

    static std::vector<Token> lex(const std::string &text);

private:
    static const map<string, TokenType> jumpMap;
};
#endif //LEXER_H
