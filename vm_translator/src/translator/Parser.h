#ifndef PARSER_H
#define PARSER_H
#include <vector>

#include "Token.h"

using namespace std;

class Parser {
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
        int line_number = 0;
        for (int i = 0; i < tokens.size(); ++i) {
            auto t = tokens[i];
            switch (t.category) {
                // 2 types of operations
                //move operation+memory segment+ number
                case MoveOperation: {
                    break;
                }
                // al operation
                case ArithmeticOrLogicOperation: {
                    break;
                }
                default:
                    throw cpptrace::logic_error("Invalid operaiton on line " + to_string(line_number));
                //Do we need to check if there 2 elements on stack for operation?
            }
            if (tokens[i].category != Terminal) {
                string value = tokens[i].type == Number
                                   ? to_string(tokens[i].number)
                                   : toString(tokens[i].type);

                string s = "Expected end of line or end of file but got ";
                throw cpptrace::logic_error(s + value + " on line" + to_string(line_number));
            }
        }
    };
};


#endif //PARSER_H
