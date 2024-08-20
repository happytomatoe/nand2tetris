#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble/Assembler.h>
#include <assemble/Lexer.h>
#include "StringDiff.h"
#include "../src/assemble/StringUtils.h"


void compare(const string &actual, const string &expected) {
    if (actual != expected) {
        auto [str1,str2] = StringDiff::get_diff_string(actual,
                                                       expected);
        FAIL() << "Actual:\t\t" << str1 << endl << "Expected:\t" << str2;
    }
}

void testAssemble(unique_ptr<TreeNode> &ast, const string &expected) {
    auto actual = Assembler::assemble(std::move(ast));
    compare(actual, expected);
}

std::vector<std::string> read_file(std::string file_path) {
    std::ifstream file(file_path);
    EXPECT_TRUE(file.good()) << "Failed to open file: " << file_path;
    std::vector<std::string> lines;
    std::string str;
    while (file.good() && std::getline(file, str)) {
        lines.push_back(str);
    }
    file.close();
    return lines;
}


TEST(AssembleTest, AddTest) {
    auto expected_file = read_file("../../data/add_expected.txt");
    //read file content

    string expected;
    for (const auto& s: expected_file) {
        expected += s + "\n";
    }
    compare(Assembler::assemble("../../data/add.asm"), expected);
}


TEST(AssembleTest, AtInstruction) {
    vector<Token> tokens = {
        Token(At, 0),
        Token(Number, 1, 1),
        Token(EOL, 2),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "0000000000000001");
}

TEST(AssembleTest, AssignmentToIdentifier) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Eof, 3),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1110001100100000");
}

TEST(AssembleTest, AssignmentToConstant) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(One, 2),
        Token(Eof, 3),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1110111111100000");
}

TEST(AssembleTest, AssignmentAndOperation) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(Eof, 5),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[3]);
    ast->right->left = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[4]);

    testAssemble(ast, "1110010011100000");
}

TEST(AssembleTest, AssignmentAndOperationWithConstant) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(EOL, 5),
    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[3]);
    ast->right->left = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[4]);

    testAssemble(ast, "1110001110100000");
}


TEST(AssembleTest, AssignmentAndOperationAndJump) {
    vector<Token> tokens = {
        Token(M, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(JGT, 5),
        Token(Eof, 6),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[3]);
    ast->right->left = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[4]);
    ast->right->right->right = std::make_unique<TreeNode>(tokens[5]);

    testAssemble(ast, "1110010011001001");
}


TEST(AssembleTest, AssignmentAndOperationAndJumpWithConstant) {
    vector<Token> tokens = {
        Token(A, 0),
        Token(Assignment, 1),
        Token(D, 2),
        Token(Plus, 3),
        Token(One, 4),
        Token(JEQ, 5),
        Token(Eof, 6),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[3]);
    ast->right->left = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[4]);
    ast->right->right->right = std::make_unique<TreeNode>(tokens[5]);

    testAssemble(ast, "1110011111100010");
}

//Is it valid just to have an operation?

TEST(AssembleTest, Operation) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(M, 4),
        Token(Eof, 6),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1111010011000000");
}

TEST(AssembleTest, OperationWithConstant) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(Eof, 6),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1110001110000000");
}

//generate test for operation and jump
TEST(AssembleTest, OperationAndJump) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(A, 4),
        Token(JGE, 5),
        Token(Eof, 6),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[3]);

    testAssemble(ast, "1110010011000011");
}

TEST(AssembleTest, OperationAndJumpWithConstant) {
    vector<Token> tokens = {
        Token(D, 2),
        Token(Minus, 3),
        Token(One, 4),
        Token(JLT, 5),
        Token(Eof, 6),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[3]);

    testAssemble(ast, "1110001110000100");
}

TEST(AssembleTest, Constant) {
    vector<Token> tokens = {
        Token(NegativeOne, 0),
        Token(Eof, 2),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);

    testAssemble(ast, "1110111010000000");
}


TEST(AssembleTest, ConstantAndJump) {
    vector<Token> tokens = {
        Token(Zero, 0),
        Token(JNE, 1),
        Token(Eof, 2),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "1110101010000101");
}

TEST(AssembleTest, ConstantAndJump2) {
    vector<Token> tokens = {
        Token(Zero, 0),
        Token(JLE, 1),
        Token(Eof, 2),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "1110101010000110");
}

TEST(AssembleTest, ConstantAndJump3) {
    vector<Token> tokens = {
        Token(Zero, 0),
        Token(JMP, 1),
        Token(Eof, 2),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "1110101010000111");
}
