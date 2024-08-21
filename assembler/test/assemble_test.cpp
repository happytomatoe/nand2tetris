#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble/Assembler.h>
#include <assemble/Lexer.h>
#include "StringDiff.h"

void compare(const string &actual, const string &expected) {
    if (actual != expected) {
        auto diff = StringDiff::get_diff_multi_line_strings(actual, expected, true);

        FAIL() << "Diff:\t\t\n" << diff ;
    }
}

void testAssemble(const unique_ptr<TreeNode> &ast, const string &expected) {
    auto actual = Assembler::assemble(ast);
    compare(actual, expected);
}

std::vector<std::string> read_file(const std::string &file_path) {
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

const string data_dir = "./data/";


void test_assemble(const string &input_file_name, const string &expected_file_name) {
    auto expected_file_lines = read_file(data_dir + expected_file_name);

    //read file content

    string expected;
    for (const auto &s: expected_file_lines) {
        expected += s + "\n";
    }
    compare(Assembler::assemble(data_dir + input_file_name), expected);
}


TEST(AssembleTest, MaxLTest) {
    test_assemble("MaxL.asm", "MaxL.hack");
}

TEST(AssembleTest, PongLTest) {
    test_assemble("PongL.asm", "PongL.hack");
}

TEST(AssembleTest, RectLTest) {
    test_assemble("RectL.asm", "RectL.hack");
}

TEST(AssembleTest, AddTest) {
    test_assemble("Add.asm", "Add.hack");
}


TEST(AssembleTest, MaxTest) {
    test_assemble("MaxL.asm", "Max.hack");
}

// TEST(AssembleTest, PongTest) {
//     test_assemble("Pong.asm", "Pong.hack");
// }

TEST(AssembleTest, RectTest) {
    test_assemble("Rect.asm", "Rect.hack");
}


TEST(AssembleTest, AtInstruction) {
    vector<Token> tokens = {
        Token(At),
        Token(Number,  1),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "0000000000000001");
}

TEST(AssembleTest, AssignmentToIdentifier) {
    vector<Token> tokens = {
        Token(A),
        Token(Assignment),
        Token(D),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1110001100100000");
}

TEST(AssembleTest, AssignmentToConstant) {
    vector<Token> tokens = {
        Token(A),
        Token(Assignment),
        Token(One),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1110111111100000");
}

TEST(AssembleTest, AssignmentAndOperation) {
    vector<Token> tokens = {
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(A),
        Token(EOL),

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
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(One),
        Token(EOL),
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
        Token(M),
        Token(Assignment),
        Token(D),
        Token(Minus),
        Token(A),
        Token(JGT),
        Token(EOL),

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
        Token(A),
        Token(Assignment),
        Token(D),
        Token(Plus),
        Token(One),
        Token(JEQ),
        Token(EOL),

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
        Token(D),
        Token(Minus),
        Token(M),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1111010011000000");
}

TEST(AssembleTest, OperationWithConstant) {
    vector<Token> tokens = {
        Token(D),
        Token(Minus),
        Token(One),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);

    testAssemble(ast, "1110001110000000");
}

//generate test for operation and jump
TEST(AssembleTest, OperationAndJump) {
    vector<Token> tokens = {
        Token(D),
        Token(Minus),
        Token(A),
        Token(JGE),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[3]);

    testAssemble(ast, "1110010011000011");
}

TEST(AssembleTest, OperationAndJumpWithConstant) {
    vector<Token> tokens = {
        Token(D),
        Token(Minus),
        Token(One),
        Token(JLT),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[1]);
    ast->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[2]);
    ast->right->right = std::make_unique<TreeNode>(tokens[3]);

    testAssemble(ast, "1110001110000100");
}

TEST(AssembleTest, Constant) {
    vector<Token> tokens = {
        Token(NegativeOne),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);

    testAssemble(ast, "1110111010000000");
}


TEST(AssembleTest, ConstantAndJump) {
    vector<Token> tokens = {
        Token(Zero),
        Token(JNE),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "1110101010000101");
}

TEST(AssembleTest, ConstantAndJump2) {
    vector<Token> tokens = {
        Token(Zero),
        Token(JLE),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "1110101010000110");
}

TEST(AssembleTest, ConstantAndJump3) {
    vector<Token> tokens = {
        Token(Zero),
        Token(JMP),
        Token(EOL),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[1]);

    testAssemble(ast, "1110101010000111");
}

TEST(AssembleTest, MMinusOne) {
    //AM=M-1
    vector<Token> tokens = {
        Token(A),
        Token(M),
        Token(Assignment),
        Token(M),
        Token(Minus),
        Token(One),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[2]);
    ast->left = std::make_unique<TreeNode>(tokens[1]);
    ast->left->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[4]);
    ast->right->left = std::make_unique<TreeNode>(tokens[3]);
    ast->right->right = std::make_unique<TreeNode>(tokens[5]);

    testAssemble(ast, "1111110010101000");
}

TEST(AssembleTest, MDMinusOne) {
    //MD=M-1
    vector<Token> tokens = {
        Token(M),
        Token(D),
        Token(Assignment),
        Token(M),
        Token(Minus),
        Token(One),

    };
    unique_ptr<TreeNode> ast = make_unique<TreeNode>(tokens[2]);
    ast->left = std::make_unique<TreeNode>(tokens[1]);
    ast->left->left = std::make_unique<TreeNode>(tokens[0]);
    ast->right = std::make_unique<TreeNode>(tokens[4]);
    ast->right->left = std::make_unique<TreeNode>(tokens[3]);
    ast->right->right = std::make_unique<TreeNode>(tokens[5]);

    testAssemble(ast, "1111110010011000");
}
