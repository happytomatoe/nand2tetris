#pragma once
#include <map>
#include <cpptrace/cpptrace.hpp>

#include "Token.h"
using namespace std;

namespace memory {
    enum MemorySegment {
        Stack,
        Local,
        Arg,
        This,
        That,
        Static,
        Constant,
        Temp,
        Pointer,
    };

    //both min and max is inclusive
    struct Range {
        int min, max;
    };


    const map<MemorySegment, Range> memorySegmentMinMaxAdress = {
        {Pointer, {3, 4}},
        {Temp, {5, 12}},

        {Static, {16, 255}},
        {Stack, {256, 2047}},
        //Dynamically allocated
        {Local, {2048, 2099}},
        {Arg, {3000, 3999}},
        {This, {4000, 4999}},
        {That, {5000, 5999}},
        // we don't save const in memory. It is only used to get a value and push
        // it onto a stack
    };
    const map<MemorySegment, int> symbolAdress = {
        {Stack, 0},
        {Local, 1},
        {Arg, 2},
        {This, 3},
        {That, 4},
    };

    const map<token::TokenType, MemorySegment> tokenTypeToMemorySegment = {
        {token::Local, Local},
        {token::Argument, Arg},
        {token::This, This},
        {token::That, That},
        {token::Pointer, Pointer},
        {token::Temp, Temp},
        {token::Static, Static},
        {token::Constant, Stack},
    };

    inline MemorySegment getMemorySegment(const token::TokenType &t) {
        if (!tokenTypeToMemorySegment.contains(t)) {
            throw cpptrace::logic_error("No corresponding segment pointer for token type " +
                                        token::toString(t));
        }
        return tokenTypeToMemorySegment.at(t);
    }


    inline Range getMemorySegmentMinMaxAdress(const MemorySegment &p) {
        if (!memorySegmentMinMaxAdress.contains(p)) {
            throw cpptrace::invalid_argument("Invalid memory segment pointer");
        }
        return memorySegmentMinMaxAdress.at(p);
    }

    inline int getSymbolAdress(const MemorySegment &p) {
        if (!symbolAdress.contains(p)) {
            throw cpptrace::invalid_argument("No symbol for memory segment pointer " + p);
        }
        return symbolAdress.at(p);
    }

    //TODO: find better option than to inline
    inline string toString(const MemorySegment p) {
        switch (p) {
            case Stack:
                return "SP";
            case Local:
                return "LCL";
            case Arg:
                return "ARG";
            case This:
                return "THIS";
            case That:
                return "THAT";
            case Static:
                return "Static";
            case Constant:
                return "Const";
            case Temp:
                return "Temp";
            case Pointer:
                return "Pointer";
            default:
                throw cpptrace::runtime_error("Invalid memory segment pointer");
        }
    }
}
