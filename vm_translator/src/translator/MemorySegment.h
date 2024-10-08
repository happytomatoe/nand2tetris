#pragma once
#include <map>
#include <ostream>
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

        friend std::ostream & operator<<(std::ostream &os, const Range &obj) {
            return os
                   << "min: " << obj.min
                   << " max: " << obj.max;
        }

        friend bool operator==(const Range &lhs, const Range &rhs) {
            return lhs.min == rhs.min
                   && lhs.max == rhs.max;
        }

        friend bool operator!=(const Range &lhs, const Range &rhs) {
            return !(lhs == rhs);
        }
    };


    const map<MemorySegment, Range> default_memory_segment_min_max_adress = {
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
    const map<MemorySegment, int> symbol_adress = {
        {Stack, 0},
        {Local, 1},
        {Arg, 2},
        {This, 3},
        {That, 4},
    };

    const unordered_map<token::TokenType, MemorySegment> token_type_to_memory_segment = {
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
        if (!token_type_to_memory_segment.contains(t)) {
            throw cpptrace::logic_error("No corresponding segment pointer for token type " +
                                        token::toString(t));
        }
        return token_type_to_memory_segment.at(t);
    }




    inline int getSymbolAdress(const MemorySegment &p) {
        if (!symbol_adress.contains(p)) {
            throw cpptrace::invalid_argument("No symbol for memory segment pointer " + p);
        }
        return symbol_adress.at(p);
    }

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
