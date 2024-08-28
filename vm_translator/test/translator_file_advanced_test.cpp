#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "translator/Translator.h"
#include <filesystem>
#include <fstream>

using namespace std;


void test_advanced(string base_file_name, const string &base_dir = "../../data/advanced/") {
    string dir_path = "out";
    if (!std::filesystem::is_directory(dir_path)) {
        if (filesystem::create_directory(dir_path))
            std::cout << "Created a directory\n";
        else
            throw cpptrace::exception_with_message("Failed to create a directory");
    }
    string file_name = base_dir + format("{}.vm", base_file_name);
    auto res = Translator::translate(file_name);
    string output_file = "out/" + base_file_name + ".asm";
    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        out_file << res;
    }
}

vector<string> files_advanced = {
    "BasicLoop/BasicLoop",
    "BasicLoop/BasicLoopExtended",
    "FibonacciElement/Main",
    "FibonacciSeries/FibonacciSeries", "NestedCall/NestedCall", "SimpleFunction/SimpleFunction",
    "StaticsTest/SimpleFunction"
};


TEST(FileTest, LoopTest) {
    test_advanced(files_advanced[0]);
}

TEST(FileTest, LoopTestExtended) {
    test_advanced(files_advanced[1]);
}
TEST(FileTest, FibonacciElement) {
    test_advanced(files_advanced[2]);
}