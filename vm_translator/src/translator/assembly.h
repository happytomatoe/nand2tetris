#pragma once
#include <string>

#include "Token.h"
using namespace std;

namespace assembly {
    enum Jump {
        JGT,
        JGE,
        JEQ,
        JNE,
        JLT,
        JLE,
        JMP,
    };

    static constexpr string toString(Jump jump) {
        switch (jump) {
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
            default:
                throw runtime_error("Invalid jump type");
        }
    };

    static Jump tokenTypeToJumpType(token::TokenType t) {
        switch (t) {
            case token::GreaterThan:
                return JGT;
            case token::LessThan:
                return JLT;
            case token::Equals:
                return JEQ;
            default:
                throw runtime_error("Invalid jump type");
        }
    };
};
