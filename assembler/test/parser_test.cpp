#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble.h>
#include <Lexer.h>


TEST(ParserTest, BasicTest) {
    std::vector<Token> tokens = {
        Token(Type::At, "@", 0),
        Token(Type::Constant, "-212313211", 1)
    };

}
