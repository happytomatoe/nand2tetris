#pragma once
#include <vector>

#include "exception.h"
#include "Token.h"

using namespace std;

class OrderChecker {
public:
    /**
     *
push constant 21
push constant 22
add
pop constant 21
     */
    static void checkOrder(const vector<Token> &tokens) {
        //check of order of tokens
        const auto end = tokens.end();
        auto line_number = 0;
        for (auto it = tokens.begin(); it != end; ++it, line_number++) {
            switch (it->category) {
                // 2 types of operations
                //move operation+memory segment+ number
                case MoveOperation: {
                    eat(it->category, it, end, line_number);
                    eat(MemorySegment, it, end, line_number);
                    eat(NumberCategory, it, end, line_number);
                    break;
                }
                // AL operation
                case ArithmeticOrLogicOperation: {
                    eat(it->category, it, end, line_number);
                    break;
                }
                default:
                    throw InvalidOperation("Invalid operaiton on line " + to_string(line_number));
                //Do we need to check if there 2 elements on stack for operation?
            }
            if (it->category != Terminal) {
                const string value = it->type == Number
                                         ? to_string(it->number)
                                         : toString(it->type);

                const string s = "Expected end of line or end of file but got ";
                throw UnexpectedToken(s + value + " on line" + to_string(line_number));
            }
        }
    };

    static void eat(const Category category, vector<Token>::const_iterator &it,
                    const vector<Token>::const_iterator end, const int &line_number) {
        if (it == end) {
            const string s = "Unexpected end of input, expected ";
            throw UnexpectedToken(s + toString(category) + "on line: " + to_string(line_number));
        }
        if (it->category != category) {
            const string s = "expected ";
            throw UnexpectedToken(
                s + toString(category) + " but got " + toString(it->type) + " on line: " + to_string(line_number));
        }
        ++it;
    }
};
