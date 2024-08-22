#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "translator/Lexer.h"
#include "StringDiff.h"
#include "Utils.h"
using namespace std;

TEST(LexerTest, BasicTest) {
    auto text = "push constant 21 //comment \npush local 22\nadd";
    vector<Token> expected = {
        Token(Push), Token(Constant), Token(Number, 21), Token(EOL),
        Token(Push), Token(Local), Token(Number, 22), Token(EOL),
        Token(Add), Token(Eof)
    };
    auto tokens = Lexer::lex(text);
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::get_diff_single_line_strings(Utils::join(tokens, ", "),
                                                                    Utils::join(expected, ", "));
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}
