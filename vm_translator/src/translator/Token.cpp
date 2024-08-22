#include "Token.h"

#include <map>
#include <ostream>
#include <cpptrace/cpptrace.hpp>
using namespace std;

Category getCategory(const TokenType &type) {
    switch (type) {
        case Add:
        case Subtract:
        case Negate:
        case Equals:
        case Greater:
        case LessThan:
        case And:
        case Or:
        case Not:
            return ArithmeticOperation;
        case Push:
        case Pop:
            return MoveOperation;
        case Local:
        case Argument:
        case This:
        case That:
        case Constant:
        case Static:
        case Temp:
        case Pointer:
            return MemorySegment;
        case Number:
            return NumberCategory;
        case Eof:
        case EOL:
            return Terminal;
        default:
            throw cpptrace::logic_error("Unknown token type: " + std::to_string((int) type));
    }
}


string toString(const Category category) {
    switch (category) {
        case ArithmeticOperation:
            return "ArithmeticOperation";
        case MoveOperation:
            return "MoveOperation";
        case MemorySegment:
            return "MemorySegment";
        case NumberCategory:
            return "NumberCategory";
        case Terminal:
            return "Terminal";
        default:
            throw cpptrace::logic_error("Unknown category: " + std::to_string((int) category));
    }
}

inline map<TokenType, string> invertMap(const map<string, TokenType> &tokenTypeByString) {
    map<TokenType, string> res;
    for (const auto &[key, value]: tokenTypeByString) {
        res[value] = key;
    }
    return res;
}

const auto tokenTypeToString = invertMap(tokenTypeMap);

string toString(const TokenType &t) {
    if (tokenTypeToString.contains(t)) {
        return tokenTypeToString.at(t);
    }
    switch (t) {
        case Eof:
            return "Eof";
        case EOL:
            return "EOL";
        case Number:
            return "Number";
        default:
            throw cpptrace::logic_error("Unknown token type: " + std::to_string(t));
    }
}
