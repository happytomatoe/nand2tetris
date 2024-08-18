#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble.h>
#include <Lexer.h>


TEST(ParserTest, BasicTest) {
    std::vector<Token> tokens = {
        Token(At, "@", 0),
        Token(Constant, "-212313211", 1)
    };
}
