#include <cstdint>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <assemble.h>
#include <Lexer.h>


std::vector<std::string> read_file(std::string file_path) {
    std::ifstream file(file_path);
    EXPECT_TRUE(file.good()) << "Failed to open file: " << file_path;
    std::vector<std::string> lines;
    std::string str;
    while (file.good() && std::getline(file, str))
    {
        lines.push_back(str);
    }
    file.close();
    return lines;
}


TEST(AssembleTest, TrimLines) {
    auto text = read_file("../data/add.asm");
    auto expected = read_file("../data/add_expected.txt");
    auto actual = assemble(text);
    for (auto s: actual) {
        std::cout<< s << std::endl;
    }
    EXPECT_EQ(actual, expected);

}
