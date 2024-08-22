#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "translator/OrderChecker.h"
#include "StringDiff.h"
using namespace std;

TEST(OrderCheckerTest, BasicTest) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 21), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    OrderChecker::checkOrder(tokens);
}

TEST(OrderCheckerTest, InvalidALOperationOrder) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 21), Token(EOL),
        Token(Push), Token(Number, 22), Token(Local), Token(EOL),
        Token(Add), Token(Eof)
    };
    EXPECT_THROW({ OrderChecker::checkOrder(tokens); }, UnexpectedToken);
}

TEST(OrderCheckerTest, InvalidMoveOperationOrder) {
    vector<Token> tokens = {
        Token(Push), Token(Number, 21), Token(Constant), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    EXPECT_THROW({ OrderChecker::checkOrder(tokens); }, UnexpectedToken);
}

TEST(OrderCheckerTest, InvalidPush) {
    vector<Token> tokens = {
        Token(Push), Token(Number, 21), Token(Constant), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    EXPECT_THROW({ OrderChecker::checkOrder(tokens); }, UnexpectedToken);
}
