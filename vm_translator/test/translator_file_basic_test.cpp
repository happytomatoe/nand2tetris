#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "translator/Translator.h"
#include <filesystem>
#include <fstream>

using namespace std;


void test_simple(string base_file_name, const string &base_dir = "../../data/simple/") {
    string dir_path = "out";
    if (!std::filesystem::is_directory(dir_path)) {
        if (filesystem::create_directory(dir_path))
            std::cout << "Created a directory\n";
        else
            throw cpptrace::exception_with_message("Failed to create a directory");
    }
    string file_name = format("{}{}/{}.vm", base_dir, base_file_name, base_file_name);
    auto res = Translator::translate(file_name);
    string output_file = "out/" + base_file_name + ".asm";
    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        out_file << res;
    }
}

vector<string> files_simple = {"BasicTest", "PointerTest", "SimpleAdd", "StackTest", "StaticTest"};

TEST(FileTest, BasicFileTest) {
    test_simple(files_simple[0]);
}

TEST(FileTest, PointerTest) {
    test_simple(files_simple[1]);
}

TEST(FileTest, SimpleAdd) {
    test_simple(files_simple[2]);
}

TEST(FileTest, StackTest) {
    test_simple(files_simple[3]);
}

TEST(FileTest, StaticTest) {
    test_simple(files_simple[4]);
}
