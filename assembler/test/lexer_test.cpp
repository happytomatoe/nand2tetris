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
        Token(At),
        Token(Number, 32767),
        Token(EOL),
        Token(A),
        Token(M),
        Token(D),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(A),
        Token(JGE),
        Token(EOL),
        Token(Eof),
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
    vector expected = {Token(At), Token(Symbol,  "R1"), Token(Eof)};
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}

TEST(LexerTest, SymbolTest) {
    auto text = "@Ra1_.$:";
    vector expected = {Token(At), Token(Symbol,  "Ra1_.$:"), Token(Eof)};
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}


TEST(LexerTest, InvalidSymbol1) {
    auto text = "@1R";
    vector expected = {Token(At), Token(Symbol,   "R1"), Token(Eof)};
    EXPECT_THROW({ Lexer::lex(text); }, invalid_symbol_exception);
}

TEST(LexerTest, MinusOne) {
    auto text = "M=M-1";
    vector expected = {
        Token(M), Token(Assignment), Token(M),
        Token(Minus), Token(One), Token(Eof)
    };
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}

TEST(LexerTest, SectionTest) {
    auto text = "(END)";
    vector expected = {
        Token(Label,  "END"),
        Token(Eof)
    };
    auto tokens = Lexer::lex(text);
    assert_eq(tokens, expected);
}
