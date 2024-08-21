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


TEST(LexerTest, BasicTest) {
    auto text = "@32767\nAMD = D  - A;JGE\n";
    vector<Token> expected = {
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
    };
    auto tokens = Lexer::lex(text);
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::get_diff_single_line_strings(Utils::join(tokens, ", "),
                                                     Utils::join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}


TEST(LexerTest, ShortOverflowTest) {
    auto text = "@65536";
    EXPECT_THROW({
                 auto tokens = Lexer::lex(text);
                 }, number_overflow_exception);
}

void assert_eq(const vector<Token> &tokens, const vector<Token> &expected) {
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::get_diff_single_line_strings(Utils::join(tokens, ", "),
                                                     Utils::join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}

TEST(LexerTest, BasicSymbolTest) {
    auto text = "@R1";
    vector expected = {Token(At, 0), Token(Symbol, 1, 0, "R1"), Token(Eof, 3)};
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}

TEST(LexerTest, SymbolTest) {
    auto text = "@Ra1_.$:";
    vector expected = {Token(At, 0), Token(Symbol, 1, 0, "Ra1_.$:"), Token(Eof, 8)};
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}


TEST(LexerTest, InvalidSymbol1) {
    auto text = "@1R";
    vector expected = {Token(At, 0), Token(Symbol, 1, 0, "R1"), Token(Eof, 3)};
    EXPECT_THROW({ Lexer::lex(text); }, invalid_symbol_exception);
}

TEST(LexerTest, MinusOne) {
    auto text = "M=M-1";
    vector expected = {
        Token(M, 0), Token(Assignment, 1), Token(M, 2),
        Token(Minus, 3), Token(One, 4), Token(Eof, 5)
    };
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}

TEST(LexerTest, SectionTest) {
    auto text = "(END)";
    vector expected = {
        Token(Label, 1, 0, "END"),
        Token(Eof, 5)
    };
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}
