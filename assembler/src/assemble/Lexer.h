//

//

#ifndef LEXER_H
#define LEXER_H
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <cpptrace/cpptrace.hpp>

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
    Label,
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
    LabelCategory,
    Identifier, // D, A, M
    Jump,
    End,
    SymbolCategory,
    Other
};

struct Token {

    //TODO: make this read only
   Category category;
   TokenType type;
   int constValue;
   string text;

    explicit Token(const TokenType type, int constValue): type(type), category(getCategory(type)),
                                                          constValue(constValue) {
        if (constValue != 0 && type != Number) {
            string s = "Expected number but got ";
            throw cpptrace::logic_error(s + Token::toString(type));
        }
    }

    explicit Token(TokenType type, string text): type(type), category(getCategory(type)), constValue(0),
                                                 text(std::move(text)) {
    }


    explicit Token(TokenType type)
        : type(type),
          category(getCategory(type)),
          constValue(0) {
    }


    explicit Token(Token *other)
        : type(other->type),
          category(other->category),
          constValue(other->constValue), text(other->text) {
    }

    Token(const Token& other) = default;
    Token& operator=(const Token& other) = default;

    friend bool operator==(const Token &lhs, const Token &rhs) {
        return lhs.category == rhs.category
               && lhs.type == rhs.type
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
            case Label:
                return "Label";
            default:
                return "Unknown";
        }
    }

private:
    static constexpr const Category getCategory(TokenType type) {
        switch (type) {
            case EOL:
                return End;
            case Label:
                return LabelCategory;
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
            case LabelCategory:
                return "LabelCategory";
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

    static tuple<string, int, bool> scan_symbol(const std::string &text, int i, const set<char> &stop_symbols);

    static std::vector<Token> lex(const std::string &text);

private:
    static const map<string, TokenType> jumpMap;
};
#endif //LEXER_H
