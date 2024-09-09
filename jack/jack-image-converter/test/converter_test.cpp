#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "converter/ImageConverter.hpp"
#include "converter/StringUtils.h"


// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}


string normalize(string str) {
    set empty_line = {'\n', ' ', '\t'};
    auto strings = StringUtils::split(str, '\n');
    string res;
    for (auto &line: strings) {
        ltrim(line);
        rtrim(line);
        if (set line_distinct_chars(line.begin(), line.end()); !ranges::includes(empty_line, line_distinct_chars)) {
            res += line + "\n";
        }
    }
    return res;
}


TEST(ConverterTest, BasicTest) {
    string expected = R"(
        method void draw(int x, int y) {
           do Screen.drawPixel(x,y);
           do Screen.drawPixel(x+1,y+1);
           do Screen.drawPixel(x+2,y+2);
           do Screen.drawPixel(x+3,y+3);
           do Screen.drawPixel(x+4,y+4);
           return;
        }
        )";
    auto res = ImageConverter::convert("data/test.png", true, true, "Screen.drawPixel", false);
    auto n1 = normalize(res);
    auto n2 = normalize(expected);
    cout << "Actual: " << endl << n1 << endl << "Expected: " << endl << n2 << endl;
    ASSERT_EQ(n1, n2);
}

