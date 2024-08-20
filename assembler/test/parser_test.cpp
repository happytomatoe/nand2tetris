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
#include <assemble/exception.h>

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

TEST(ParserTest, AssignmentForMultipleIdentifiers) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(M, 1),
        Token(D, 2),
        Token(Assignment, 3),
        Token(D, 4),
        Token(EOL, 5),

    };
    TreeNode expected = TreeNode(tokens[3]);
    expected.left = std::make_unique<TreeNode>(tokens[1]);
    expected.left->left = std::make_unique<TreeNode>(tokens[0]);
    expected.left->right = std::make_unique<TreeNode>(tokens[2]);
    expected.right = std::make_unique<TreeNode>(tokens[4]);

    test_parser(tokens, expected);
}

TEST(ParserTest, InvalidIdentifiersOrder1) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(D, 1),
        Token(M, 2),
        Token(Assignment, 3),
        Token(D, 4),
        Token(EOL, 5),

    };
    EXPECT_THROW({
                 auto p = new Parser(tokens);
                 auto const actual = p->parse();
                 }, invalid_identifiers_order_before_assignment_exception);
}

TEST(ParserTest, InvalidIdentifiersOrder2) {
    vector<Token> tokens = {
        Token(M, 0),
        Token(A, 1),
        Token(D, 2),
        Token(Assignment, 3),
        Token(D, 4),
        Token(EOL, 5),

    };
    EXPECT_THROW({
                 auto p = new Parser(tokens);
                 auto const actual = p->parse();
                 }, invalid_identifiers_order_before_assignment_exception);
}

TEST(ParserTest, InvalidIdentifiersOrder3) {
    vector<Token> tokens = {
        Token(D, 0),
        Token(A, 1),
        Token(Assignment, 3),
        Token(D, 4),
        Token(EOL, 5),
    };
    EXPECT_THROW({
                 auto p = new Parser(tokens);
                 auto const actual = p->parse();
                 }, invalid_identifiers_order_before_assignment_exception);
};
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

TEST(ParserTest, InvalidOperandOrder) {
    vector<Token> tokens = {
        Token(A, 2),
        Token(Minus, 3),
        Token(D, 4),
        Token(EOL, 6),

    };
    EXPECT_THROW({
                 auto p = new Parser(tokens);
                 auto const actual = p->parse();
                 cout << actual << endl;
                 }, invalid_operand_order_exception);
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

class InvalidOperationWithNegativeOneTest : public testing::TestWithParam<tuple<TokenType, TokenType> > {
};

TEST_P(InvalidOperationWithNegativeOneTest, ControlInstructionInvalid2) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(get<0>(GetParam()), 2),
        Token(get<1>(GetParam()), 2),
        Token(EOL, 5),

    };
    EXPECT_THROW({
                 auto p = new Parser(tokens);
                 auto const actual = p->parse();
                 cout << actual << endl;
                 }, syntax_error_exception);
}

const auto operations = testing::Values(Plus, Minus, And, Or, Not);
const auto operands = testing::Values(NegativeOne, Zero);

inline constexpr const char *ToString(TokenType v);

INSTANTIATE_TEST_SUITE_P(MyGroup, InvalidOperationWithNegativeOneTest,
                         testing::Combine(operations,operands),
                         [](const testing::TestParamInfo<InvalidOperationWithNegativeOneTest::ParamType>& info) {
                         string name;
                         name += ToString(get<0>(info.param));
                         name += ToString(get<1>(info.param));
                         return name;
                         });


void sigsegv_handler(int sig) {
    cpptrace::generate_trace().print();
    exit(1);
}


int main(int argc, char **argv) {
    signal(SIGSEGV, sigsegv_handler);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//TODO: fix +0


inline constexpr const char *ToString(TokenType v) {
    switch (v) {
        case Plus:
            return "Plus";
        case Minus:
            return "Minus";
        case Assignment:
            return "Assignment";
        case And:
            return "And";
        case Or:
            return "Or";
        case Not:
            return "Not";
        case D:
            return "D";
        case A:
            return "A";
        case M:
            return "M";
        case Number:
            return "Constant";
        case NegativeOne:
            return "NegativeOne";
        case Zero:
            return "Zero";
        case One:
            return "One";
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
        case At:
            return "At";
        case EOL:
            return "EOL";
        default:
            return "Unknown";
    }
}
