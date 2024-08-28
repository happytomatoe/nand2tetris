#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "translator/Lexer.h"
#include "StringDiff.h"
#include "Utils.h"
using namespace std;

TEST(LexerTest, BasicTest) {
    auto text = R"(
function Main.fibonacci 0
	push argument 0
	push constant 2
	lt
	if-goto N_LT_2
	goto N_GE_2
label N_LT_2               // if n < 2 returns n
	push argument 0
	return
label N_GE_2               // if n >= 2 returns fib(n - 2) + fib(n - 1)
	push argument 0
	push constant 2
	sub
	call Main.fibonacci 1  // computes fib(n - 2)
	push argument 0
	push constant 1
	sub
	call Main.fibonacci 1  // computes fib(n - 1)
	add                    // returns fib(n - 1) + fib(n - 2)
	return
)";
    vector<Token> expected = {
        Token(EOL),
        Token(Function, "function", 0), Token(EOL),
        Token(Push), Token(Argument), Token(Number, 0), Token(EOL),
        Token(Push), Token(Constant), Token(Number, 2), Token(EOL),
        Token(LessThan), Token(EOL),
        Token(IfGoto, "N_LT_2"), Token(EOL),
        Token(Goto, "N_GE_2"), Token(EOL),
        Token(Label, "N_LT_2"), Token(EOL),
        Token(Push), Token(Argument), Token(Number, 0), Token(EOL),
        Token(Return), Token(EOL),
        Token(Label, "N_GE_2"), Token(EOL),
        Token(Push), Token(Argument), Token(Number, 0), Token(EOL),
        Token(Push), Token(Constant), Token(Number, 2), Token(EOL),
        Token(Subtract), Token(EOL),
        Token(Call, "Main.fibonacci", 1), Token(EOL),
        Token(Push), Token(Argument), Token(Number, 0), Token(EOL),
        Token(Push), Token(Constant), Token(Number, 1), Token(EOL),
        Token(Subtract), Token(EOL),
        Token(Call, "Main.fibonacci", 1), Token(EOL),
        Token(Add), Token(EOL),
        Token(Return), Token(EOL), Token(Eof)
    };
    auto tokens = Lexer::lex(text);
    if (tokens != expected) {
        auto [str1,str2] = StringDiff::diff(tokens, expected);
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}
