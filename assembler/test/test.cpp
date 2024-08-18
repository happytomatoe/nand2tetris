//
// #include <string>
// #include "assemble.hpp"
//
// int main() {
//     std::string text = R"""(
//         add r0, r1, r2;
//         """)
//     std::string assembledText = assemble(text);
//     if (assembledText == "add r0, r1, r2") {
//         return 0;
//     } else {
//         return 1;
//     }
// }


#include <cstdint>
#include <fstream>
#include <iostream>

#include "assemble.hpp"

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}


std::string read_file(std::string file_path);

// TEST_CASE( "Test assemble lines" ) {
//     INFO("Test case start");
//     std::string text = read_file("/home/babkamen/git/nand2tetris/nand2tetris/assembler/data/add.asm");
//     std::string expected = read_file("./data/add_expeced.txt");
//     std::cout << text;
//     // auto actual = assemble(text);
//     CHECK_THAT(text, Catch::Matchers::Equals(expected,Catch::CaseSensitive::Yes));
//
// }

// std::string read_file(std::string file_path) {
//     std::ifstream file(file_path);
//     REQUIRE(file);
//     std::vector<std::string> lines;
//     std::string str;
//     std::string file_contents;
//     while (std::getline(file, str))
//     {
//         file_contents += str;
//         file_contents += "\n";
//     }
//     file.close();
//     return file_contents;
// }