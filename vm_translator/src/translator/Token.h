#pragma once
#include <map>
#include <ostream>
#include <cpptrace/cpptrace.hpp>
using namespace std;

namespace token {
    enum TokenType {
        //AL operations
        Add,
        Subtract,
        Negate,
        Equals,
        Greater,
        LessThan,
        And,
        Or,
        Not,
        //Move operations
        Push,
        Pop,
        //Memory segments
        Local,
        Argument,
        This,
        That,
        Temp,
        Pointer,
        ConstantMemorySegment,
        StaticMemorySegment,

        //
        Number,
        // Terminal
        Eof,
        EOL,
    };


    enum Category {
        MemorySegmentCategory,
        MoveOperation,
        ArithmeticOrLogicOperation,
        NumberCategory,
        Terminal
    };

    Category getCategory(const TokenType &type);

    string toString(Category category);

    string toString(const TokenType &t);

    struct Token {
        TokenType type;
        Category category;
        int number;

        explicit Token(TokenType type)
            : type(type),
              category(getCategory(type)),
              number(0) {
        }

        Token(TokenType type, int number)
            : type(type),
              category(getCategory(type)),
              number(number) {
        }

        friend bool operator==(const Token &lhs, const Token &rhs) {
            return lhs.type == rhs.type
                   && lhs.category == rhs.category
                   && lhs.number == rhs.number;
        }

        friend bool operator!=(const Token &lhs, const Token &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const Token &obj) {
            return os
                   << "Token(category: " << toString(obj.category)
                   << ", type: " << toString(obj.type)
                   << ", number: " << obj.number << ")";
        }
    };

    Category getCategory(TokenType type);

    const map<string, TokenType> tokenTypeMap = {
        {"add", Add},
        {"sub", Subtract},
        {"neg", Negate},
        {"eq", Equals},
        {"gt", Greater},
        {"lt", LessThan},
        {"and", And},
        {"or", Or},
        {"not", Not},
        {"push", Push},
        {"pop", Pop},
        {"local", Local},
        {"argument", Argument},
        {"this", This},
        {"that", That},
        {"constant", ConstantMemorySegment},
        {"static", StaticMemorySegment},
        {"temp", Temp},
        {"pointer", Pointer},
    };
}
