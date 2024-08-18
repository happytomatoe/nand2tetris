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
#include <gtest/gtest.h>
#include <assemble.h>


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

