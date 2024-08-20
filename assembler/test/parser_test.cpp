#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble/Lexer.h>
#include "assemble/Parser.h"
#include <stdio.h>
#include <execinfo.h>
#include <list>
#include <signal.h>
#include <cpptrace/cpptrace.hpp>

void test_parser(const vector<Token> &tokens, const TreeNode &expected) {
    auto p = new Parser(tokens);
    auto const actual = p->parse();
    cout << *actual << endl;
    EXPECT_TRUE(actual != nullptr);
    EXPECT_EQ(*actual, expected);
}


TEST(ParserTest, AInstruction) {
    vector<Token> tokens = {
        Token(At, 0),
        Token(Number, 1, 1),
        Token(EOL, 2),

    };
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[1]);

    test_parser(tokens, expected);
}

TEST(ParserTest, AssignmentToIdentifier) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(EOL, 3),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

list<TokenType> predefined_constants = {Zero, One, NegativeOne};

class ParserConstantTest : public testing::TestWithParam<TokenType> {
};

TEST_P(ParserConstantTest, AssignmentToConstant) {
    vector tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(GetParam(), 2),
        Token(EOL, 3),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

INSTANTIATE_TEST_SUITE_P(ParserTest, ParserConstantTest, testing::ValuesIn(predefined_constants));



TEST(ParserTest, AssignmentAndOperation) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(EOL, 5),
    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[3]);
    expected.right->left = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[4]);

    test_parser(tokens, expected);
}

TEST(ParserTest, AssignmentAndOperationWithConstant) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(EOL, 5),
    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[3]);
    expected.right->left = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[4]);

    test_parser(tokens, expected);
}


TEST(ParserTest, AssignmentAndOperationAndJump) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(JGE, 5),
        Token(EOL, 6),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[3]);
    expected.right->left = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[4]);
    expected.right->right->right = std::make_unique<TreeNode>(tokens[5]);

    test_parser(tokens, expected);
}

TEST(ParserTest, AssignmentAndOperationAndJumpWithConstant) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(JGE, 5),
        Token(EOL, 6),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[3]);
    expected.right->left = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[4]);
    expected.right->right->right = std::make_unique<TreeNode>(tokens[5]);

    test_parser(tokens, expected);
}

//Is it valid just to have an operation?

TEST(ParserTest, Operation) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(EOL, 6),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

TEST(ParserTest, OperationWithConstant) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(EOL, 6),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

//generate test for operation and jump
TEST(ParserTest, OperationAndJump) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(JGE, 5),
        Token(EOL, 6),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[3]);

    test_parser(tokens, expected);
}

TEST(ParserTest, OperationAndJumpWithConstant) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(JGT, 5),
        Token(EOL, 6),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[3]);

    test_parser(tokens, expected);
}

TEST(ParserTest, Constant) {
    vector<Token> tokens = {
        Token(NegativeOne, 0),
        Token(EOL, 2),

    };
    TreeNode expected = TreeNode(tokens[0]);

    test_parser(tokens, expected);
}


TEST(ParserTest, ConstantAndJump) {
    vector<Token> tokens = {
        Token(NegativeOne, 0),
        Token(JGE, 1),
        Token(EOL, 2),

    };
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[1]);

    test_parser(tokens, expected);
}


TEST(ParserTest, ControlInstructionInvalid) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Minus, 2),
        Token(EOL, 5),

    };
    EXPECT_THROW({
                 auto p = new Parser(tokens);
                 auto const actual = p->parse();
                 cout << actual << endl;
                 }, cpptrace::logic_error);
}


void sigsegv_handler(int sig) {
    cpptrace::generate_trace().print();
    exit(1);
}


int main(int argc, char **argv) {
    signal(SIGSEGV, sigsegv_handler);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//TODO: fix +-1
//TDO: fix +0