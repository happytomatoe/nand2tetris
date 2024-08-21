#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble/Assembler.h>
#include <assemble/Lexer.h>
#include <list>

#include "StringDiff.h"
#include "Utils.h"
#include "assemble/exception.h"
using namespace std;
vector<pair<string, vector<Token> > > input_expected_value = {
    {
        "@32767\nAMD = D  - A;JGE\n",
        {
            Token(At, 0),
            Token(Number, 1, 32767),
            Token(EOL, 6),
            Token(A, 7, 0),
            Token(M, 8, 0),
            Token(D, 9, 0),
            Token(Assignment, 11),
            Token(D, 13),
            Token(Minus, 16),
            Token(A, 18),
            Token(JGE, 22),
            Token(EOL, 23),
            Token(Eof, 24),
        }
    },

};

class LexerTest : public testing::TestWithParam<pair<string, vector<Token> > > {
};


TEST_P(LexerTest, BasicTest) {
    auto text = GetParam().first;
    auto expected = GetParam().second;
    auto tokens = Lexer::lex(text);
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::getDiffString(Utils::join(tokens, ", "),
                                                       Utils::join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}

INSTANTIATE_TEST_SUITE_P(MyGroup, LexerTest, testing::ValuesIn(input_expected_value));

TEST(LexerTest, ShortOverflowTest) {
    auto text = "@65536";
    EXPECT_THROW({
                 auto tokens = Lexer::lex(text);
                 }, number_overflow_exception);
}


TEST(LexerTest, BasicSymbolTest) {
    auto text = "@R1";
    vector expected = {Token(At, 0), Token(Symbol, 1, 0, "R1"), Token(Eof, 3)};
    auto tokens = Lexer::lex(text);
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::getDiffString(Utils::join(tokens, ", "),
                                                       Utils::join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}

TEST(LexerTest, SymbolTest) {
    auto text = "@Ra1_.$:";
    vector expected = {Token(At, 0), Token(Symbol, 1, 0, "Ra1_.$:"), Token(Eof, 8)};
    auto tokens = Lexer::lex(text);
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::getDiffString(Utils::join(tokens, ", "),
                                                       Utils::join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}


TEST(LexerTest, InvalidSymbol1) {
    auto text = "@1R";
    vector expected = {Token(At, 0), Token(Symbol, 1, 0, "R1"), Token(Eof, 3)};
    EXPECT_THROW({ Lexer::lex(text); }, invalid_symbol_exception);
}
