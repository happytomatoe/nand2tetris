#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "translator/Validator.h"
#include "StringDiff.h"
using namespace std;

TEST(ValidatorTest, BasicTest) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 21), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    Validator::checkOrder(tokens);
}

TEST(ValidatorTest, InvalidALOperationOrder) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 21), Token(EOL),
        Token(Push), Token(Number, 22), Token(Local), Token(EOL),
        Token(Add), Token(Eof)
    };
    EXPECT_THROW({ Validator::checkOrder(tokens); }, UnexpectedToken);
}

TEST(ValidatorTest, InvalidMoveOperationOrder) {
    vector<Token> tokens = {
        Token(Push), Token(Number, 21), Token(Constant), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    EXPECT_THROW({ Validator::checkOrder(tokens); }, UnexpectedToken);
}

TEST(ValidatorTest, InvalidPush) {
    vector<Token> tokens = {
        Token(Push), Token(Number, 21), Token(Constant), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    EXPECT_THROW({ Validator::checkOrder(tokens); }, UnexpectedToken);
}

TEST(ValidatorTest, NegativeNumberWithMoveOperation) {
    vector<Token> tokens = {
        Token(Push), Token(Local), Token(Number, -21) , Token(EOL),
    };
    EXPECT_THROW({ Validator::checkOrder(tokens); }, InvalidOperation);
}
