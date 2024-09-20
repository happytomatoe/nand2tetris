
#include "translator/Linker.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <cpptrace/cpptrace.hpp>
#include <filesystem>
#include <vector>

using namespace std;

TEST(LinkerTest, BasicTest) {
    string dir = "../../data/advanced/MyTest/";
    vector<filesystem::path> out_files = {dir + "Test.asm", dir + "Main.asm"};
    EXPECT_THROW(
        { Linker::interlink(move(out_files), dir + "Linked.asm"); },
        cpptrace::domain_error);
}