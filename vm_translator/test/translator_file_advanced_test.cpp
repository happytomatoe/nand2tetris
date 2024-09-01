#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "translator/Translator.h"
#include <filesystem>
#include <fstream>

using namespace std;


void create_dir_if_not_exists(string dir_path) {
    if (!std::filesystem::is_directory(dir_path)) {
        if (filesystem::create_directory(dir_path))
            std::cout << "Created a directory\n";
        else
            throw cpptrace::exception_with_message("Failed to create a directory");
    }
}

void test_advanced(string base_file_name, const bool memory_init = true,
                   const string &base_dir = "../../data/advanced/") {
    string dir_path = "out";
    create_dir_if_not_exists(dir_path);
    string file_name = base_dir + format("{}.vm", base_file_name);
    auto res = Translator().translate(file_name);
    string output_file = "out/" + base_file_name + ".asm";
    auto output_dir = output_file.substr(0, output_file.find_last_of('/'));
    if (!std::filesystem::is_directory(output_dir)) {
        if (filesystem::create_directory(output_dir))
            std::cout << "Created a directory\n";
        else
            throw cpptrace::exception_with_message("Failed to create a directory");
    }
    cout << "Base dir " << output_dir << endl;
    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        if (!out_file.is_open()) {
            throw cpptrace::exception_with_message("Failed to open file: " + output_file);
        }
        out_file << res;
        out_file.close();
    }
}


void link_together(std::initializer_list<string> files, const string &output_file, const string &base_dir = "out/") {
    ofstream out_file(base_dir + output_file + ".asm");
    if (!out_file.is_open()) {
        throw cpptrace::exception_with_message("Failed to open file: " + output_file);
    }
    for (auto file: files) {
        string file_name = base_dir + format("{}.asm", file);
        ifstream input_file(file_name);
        if (!input_file.is_open()) {
            throw cpptrace::exception_with_message("Failed to open file: " + output_file);
        }
        string line;
        while (getline(input_file, line)) {
            out_file << line << "\n";
        }
    }
    out_file.close();
}

TEST(FileAdvancedTest, LoopTest) {
    test_advanced("BasicLoop/BasicLoop");
}

TEST(FileAdvancedTest, LoopTestExtended) {
    test_advanced("BasicLoop/BasicLoopExtended");
}

TEST(FileAdvancedTest, FibonacciElement) {
    test_advanced("FibonacciElement/Main");
}

TEST(FileAdvancedTest, FibonacciSeries) {
    test_advanced("FibonacciSeries/FibonacciSeries");
}

TEST(FileAdvancedTest, NestedCalls) {
    test_advanced("NestedCall/Sys");
}

TEST(FileAdvancedTest, SimpleFunction) {
    test_advanced("SimpleFunction/SimpleFunction");
}

TEST(FileAdvancedTest, StaticTest) {
    test_advanced("StaticsTest/Class1", false);
    test_advanced("StaticsTest/Class2", false);
    test_advanced("StaticsTest/Sys");
    link_together({"StaticsTest/Sys", "StaticsTest/Class1", "StaticsTest/Class2"}, "StaticsTest/Linked");
}
