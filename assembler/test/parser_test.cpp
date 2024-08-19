#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble.h>
#include <Lexer.h>

#include "Parser.h"
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


TEST(ParserTest, BasicTest) {
    vector<Token> tokens = {
        Token(At, 0),
        Token(Number, 1, 1)
    };
    TreeNode tree = TreeNode(tokens[0]);
    tree.right = std::make_unique<TreeNode>(tokens[1]);

    auto const node = Parser::parse(&tokens);
    cout << *node;
    EXPECT_EQ(*node, tree);
}


TEST(ParserTest, ControlInstruction) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
    };
    TreeNode expected = TreeNode(tokens[1]);
    expected.left = std::make_unique<TreeNode>(tokens[0]);
    expected.right = std::make_unique<TreeNode>(tokens[3]);
    expected.right->left = std::make_unique<TreeNode>(tokens[2]);
    expected.right->right = std::make_unique<TreeNode>(tokens[4]);

    auto const actual = Parser::parse(&tokens);
    cout << *actual;
    EXPECT_EQ(*actual, expected);
}

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}