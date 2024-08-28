#pragma once
#include <map>
#include <ostream>
#include <unordered_map>
#include <cpptrace/cpptrace.hpp>
#include <utility>
using namespace std;

namespace token {
    enum TokenType {
        //AL operations
        Add,
        Subtract,
        Negate,
        Equals,
        GreaterThan,
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
        Constant,
        Static,
        //
        Number,
        // Branching
        Label,
        IfGoto,
        Goto,
        //Functions
        Function,
        Return,
        Call,
        // Terminal
        Eof,
        EOL,
    };

    enum Category {
        MemorySegment,
        MoveOperation,
        ArithmeticOrLogicOperation,
        NumberCategory,
        IfGoToCategory,
        FunctionCategory,
        GoToCategory,
        LabelCategory,
        Terminal
    };

    Category getCategory(const TokenType &type);

    string toString(Category category);

    string toString(const TokenType &t);

    struct Token {
        TokenType type;
        Category category;
        int number = 0, functionArgumentCount = -1;
        string label, functionName;

        explicit Token(const TokenType type)
            : type(type),
              category(getCategory(type)) {
        }

        Token(TokenType type, const int number)
            : type(type),
              category(getCategory(type)),
              number(number) {
        }

        Token(TokenType type, const char *functionName, int functionArgsCount): type(type),
            category(getCategory(type)),
            functionName(functionName),
            functionArgumentCount(functionArgsCount) {
        }

        Token(TokenType type, const string &label): type(type),
            category(getCategory(type)),
            label(label) {
        }

        friend bool operator==(const Token &lhs, const Token &rhs) {
            return lhs.type == rhs.type
                   && lhs.category == rhs.category
                   && lhs.number == rhs.number;
        }

        friend bool operator!=(const Token &lhs, const Token &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream & operator<<(std::ostream &os, const Token &obj) {
            return os
                   << "Token(type: " << toString(obj.type)
                   << ", category: " << toString(obj.category)
                   << ", number: " << obj.number
                   << ", functionArgumentCount: " << obj.functionArgumentCount
                   << ", label: " << obj.label
                   << ", functionName: " << obj.functionName << ")";
        }
    };

    Category getCategory(TokenType type);

    const unordered_map<string, TokenType> tokenTypeMap = {
        {"add", Add},
        {"sub", Subtract},
        {"neg", Negate},
        {"eq", Equals},
        {"gt", GreaterThan},
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
        {"constant", Constant},
        {"static", Static},
        {"temp", Temp},
        {"pointer", Pointer},
        {"label", Label},
        {"goto", Goto},
        {"if-goto", IfGoto},
        {"function", Function},
        {"return", Return},
        {"call", Call},
    };
}
