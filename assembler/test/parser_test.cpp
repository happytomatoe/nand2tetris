#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble/Lexer.h>
#include "assemble/Parser.h"
#include <cstdio>
#include <execinfo.h>
#include <list>
#include <csignal>
#include <cpptrace/cpptrace.hpp>
#include <assemble/exception.h>

#include "StringDiff.h"
#include "Utils.h"

void test_parser(const vector<Token> &tokens, const TreeNode &expected, shared_ptr<Parser> p = make_shared<Parser>()) {
    auto const actual = p->parse(tokens);
    EXPECT_TRUE(actual != nullptr);
    if (*actual != expected) {
        auto [str1,str2] =
                StringDiff::get_diff_single_line_strings(Utils::toString(*actual), Utils::toString(expected));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}


TEST(ParserTest, AInstruction) {
    vector<Token> tokens = {
        Token(At),
        Token(Number, 1),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[1]);

    test_parser(tokens, expected);
}

class PredefinedSymbolsTest : public testing::TestWithParam<pair<string, int> > {
};

TEST_P(PredefinedSymbolsTest, PredefinedSymbolsTest) {
    vector tokens = {Token(At), Token(Symbol,  GetParam().first), Token(Eof)};
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(Token(Number, GetParam().second));
    test_parser(tokens, expected);
}

list<pair<string, int> > predefined_symbols() {
    list<pair<string, int> > predefined_symbols;
    for (const auto &s: Parser::predefined_symbols) {
        predefined_symbols.emplace_back(s);
    }
    return predefined_symbols;
}

INSTANTIATE_TEST_SUITE_P(ParserTest, PredefinedSymbolsTest, testing::ValuesIn(predefined_symbols()));

TEST(ParserTest, SymbolsTest) {
    auto p = make_shared<Parser>();

    vector tokens = {
        Token(At), Token(Symbol, "@Ra1_.$:"), Token(Eof)
    };
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(Token(Number, 16));
    test_parser(tokens, expected, p);

    vector tokens2 = {
        Token(At), Token(Symbol,  "@TMP2"), Token(Eof)
    };
    TreeNode expected2 = TreeNode(tokens2[0]);
    expected2.right = make_unique<TreeNode>(Token(Number,  17));
    test_parser(tokens2, expected2, p);
}


TEST(ParserTest, AssignmentToIdentifier) {
    vector<Token> tokens = {
        Token(A),
        Token(Assignment),
        Token(D),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

TEST(ParserTest, AssignmentForMultipleIdentifiers) {
    vector<Token> tokens = {
        Token(A),
        Token(M),
        Token(D),
        Token(Assignment),
        Token(D),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[3]);
    expected.left = std::make_unique<TreeNode>(tokens[1]);
    expected.left->left = std::make_unique<TreeNode>(tokens[0]);
    expected.left->right = std::make_unique<TreeNode>(tokens[2]);
    expected.right = std::make_unique<TreeNode>(tokens[4]);

    test_parser(tokens, expected);
}

TEST(ParserTest, IdentifierJump) {
    vector<Token> tokens = {
        Token(D),
        Token(JGT),
        Token(Eof),

    };
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[1]);

    test_parser(tokens, expected);
}


TEST(ParserTest, InvalidIdentifiersOrder1) {
    vector<Token> tokens = {
        Token(A),
        Token(D),
        Token(M),
        Token(Assignment),
        Token(D),
        Token(EOL),

    };
    EXPECT_THROW({
                 auto p = new Parser();
                 auto const actual = p->parse(tokens);
                 }, invalid_identifiers_order_before_assignment_exception);
}

TEST(ParserTest, InvalidIdentifiersOrder2) {
    vector<Token> tokens = {
        Token(M),
        Token(A),
        Token(D),
        Token(Assignment),
        Token(D),
        Token(EOL),

    };
    EXPECT_THROW({
                 auto p = new Parser();
                 auto const actual = p->parse(tokens);
                 }, invalid_identifiers_order_before_assignment_exception);
}

TEST(ParserTest, InvalidIdentifiersOrder3) {
    vector<Token> tokens = {
        Token(D),
        Token(A),
        Token(Assignment),
        Token(D),
        Token(EOL),
    };
    EXPECT_THROW({ (new Parser())->parse(tokens); }, invalid_identifiers_order_before_assignment_exception);
};
list<TokenType> predefined_constants = {Zero, One, NegativeOne};

class ParserConstantTest : public testing::TestWithParam<TokenType> {
};

TEST_P(ParserConstantTest, AssignmentToConstant) {
    vector tokens = {
        Token(A),
        Token(Assignment),
        Token(GetParam()),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

INSTANTIATE_TEST_SUITE_P(ParserTest, ParserConstantTest, testing::ValuesIn(predefined_constants));


TEST(ParserTest, AssignmentAndOperation) {
    vector<Token> tokens = {
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(A),
        Token(EOL),
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
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(One),
        Token(EOL),
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
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(A),
        Token(JGE),
        Token(EOL),

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
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(One),
        Token(JGE),
        Token(EOL),

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
        Token(D),
        Token(Minus),
        Token(A),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

TEST(ParserTest, InvalidOperandOrder) {
    vector<Token> tokens = {
        Token(A),
        Token(Minus),
        Token(D),
        Token(EOL),

    };
    EXPECT_THROW({
                 (new Parser())->parse(tokens);
                 }, invalid_operand_order_exception);
}

TEST(ParserTest, OperationWithConstant) {
    vector<Token> tokens = {
        Token(D),
        Token(Minus),
        Token(One),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);

    test_parser(tokens, expected);
}

//generate test for operation and jump
TEST(ParserTest, OperationAndJump) {
    vector<Token> tokens = {
        Token(D),
        Token(Minus),
        Token(A),
        Token(JGE),
        Token(EOL),
    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[3]);

    test_parser(tokens, expected);
}

TEST(ParserTest, OperationAndJumpWithConstant) {
    vector<Token> tokens = {
        Token(D),
        Token(Minus),
        Token(One),
        Token(JGT),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[3]);

    test_parser(tokens, expected);
}

TEST(ParserTest, Constant) {
    vector<Token> tokens = {
        Token(NegativeOne),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[0]);

    test_parser(tokens, expected);
}


TEST(ParserTest, ConstantAndJump) {
    vector<Token> tokens = {
        Token(NegativeOne),
        Token(JGE),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[1]);

    test_parser(tokens, expected);
}

TEST(ParserTest, NotTest) {
    vector<Token> tokens = {
        Token(M),
        Token(Assignment),
        Token(Not),
        Token(M),
        Token(EOL),

    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[3]);

    test_parser(tokens, expected);
}

TEST(ParserTest, NotTestInvalid) {
    vector<Token> tokens = {
        Token(M),
        Token(Assignment),
        Token(Not),
        Token(Zero),
        Token(EOL),

    };

    EXPECT_THROW({ (new Parser())->parse(tokens); }, syntax_error_exception);
}


TEST(ParserTest, ControlInstructionInvalid) {
    vector<Token> tokens = {
        Token(A),
        Token(Minus),
        Token(EOL),

    };
    EXPECT_THROW({
                 (new Parser())->parse(tokens);
                 }, cpptrace::logic_error);
}

TEST(ParserTest, LabelTest) {
    auto p = make_shared<Parser>();

    vector<Token> tokens = {
        Token(Label,  "END"),
        Token(EOL),

    };
    p->parse_only_labels(tokens);
    p->reset();


    auto const actual = p->parse(tokens);
    ASSERT_EQ(actual, nullptr);


    vector tokens2 = {
        Token(At), Token(Symbol,  "@TMP2"), Token(Eof)
    };
    TreeNode expected2 = TreeNode(tokens2[0], nullptr, make_unique<TreeNode>(Token(Number,  16)));
    test_parser(tokens2, expected2, p);


    vector tokens3 = {
        Token(At), Token(Symbol,   "END"), Token(Eof)
    };
    TreeNode expected3 = TreeNode(tokens2[0], nullptr, make_unique<TreeNode>(Token(Number, 0)));

    test_parser(tokens3, expected3, p);
}


class InvalidOperationWithNegativeOneTest : public testing::TestWithParam<tuple<TokenType, TokenType> > {
};

TEST_P(InvalidOperationWithNegativeOneTest, ControlInstructionInvalid2) {
    vector<Token> tokens = {
        Token(A),
        Token(get<0>(GetParam())),
        Token(get<1>(GetParam())),
        Token(EOL),

    };
    EXPECT_THROW({
                 (new Parser())->parse(tokens);
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
        case Label:
            return "Label";
        case Eof:
            return "Eof";
        case EOL:
            return "EOL";
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
        default:
            return "Unknown";
    }
}
