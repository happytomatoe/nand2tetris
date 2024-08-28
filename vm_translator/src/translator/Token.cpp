#include "Token.h"

#include <map>
#include <cpptrace/cpptrace.hpp>
using namespace std;

namespace token {
    Category getCategory(const TokenType &type) {
        switch (type) {
            case Add:
            case Subtract:
            case Negate:
            case Equals:
            case GreaterThan:
            case LessThan:
            case And:
            case Or:
            case Not:
                return ArithmeticOrLogicOperation;
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
            case Label:
                return LabelCategory;
            case Goto:
                return GoToCategory;
            case IfGoto:
                return IfGoToCategory;
            case Return:
            case Call:
            case Function:
                return FunctionCategory;
            default:
                throw cpptrace::logic_error("Cant find category for token type " + toString(type));
        }
    }


    string toString(const Category category) {
        switch (category) {
            case ArithmeticOrLogicOperation:
                return "ArithmeticOperation";
            case MoveOperation:
                return "MoveOperation";
            case MemorySegment:
                return "MemorySegment";
            case NumberCategory:
                return "NumberCategory";
            case Terminal:
                return "Terminal";
            case IfGoToCategory:
                return "Jumps";
            case LabelCategory:
                return "Label";
            case GoToCategory:
                return "Goto";
            default:
                throw cpptrace::logic_error("Unknown category");
        }
    }

    inline unordered_map<TokenType, string> invertMap(const unordered_map<string, TokenType> &tokenTypeByString) {
        unordered_map<TokenType, string> res;
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
            case Label:
                return "Identifier";
            default:
                throw cpptrace::logic_error("Unknown token type");
        }
    }
}
