#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "translator/StringUtils.h"
#include "translator/Translator.h"

using namespace std;
TEST(TranslatorTest, BasicFileTest) {
    string file_name = "/home/babkamen/git/nand2tetris/nand2tetris/vm_translator/data/BasicTest/BasicTest.vm";
    auto s = Translator::translate(file_name);
    cout << s << endl;
}

TEST(TranslatorTest, T) {
    string s = R"(
            |123
            |abc
)";

    cout << translator::stripMargin(s);

}
