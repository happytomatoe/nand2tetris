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
using namespace std;
vector<pair<string, vector<Token> > > input_expected_value = {
    {
        "@-212313211\nM = D  - A;JGE",
        {
            Token(At, 0),
            Token(Number, 1, -212313211),
            Token(EOL, 11),
            Token(M, 12,0 ),
            Token(Assignment, 14),
            Token(D, 16),
            Token(Minus, 19),
            Token(A, 21),
            Token(JGE, 25),
            Token(Eof, 26)
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
        auto [str1,str2] = StringDiff::get_diff_string(Utils::Join(tokens, ", "),
                                                       Utils::Join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}

INSTANTIATE_TEST_SUITE_P(MyGroup, LexerTest, testing::ValuesIn(input_expected_value));
