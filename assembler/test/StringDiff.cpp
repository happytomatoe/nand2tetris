#include "StringDiff.h"

#include <iostream>
#include <string>

#define RED "\033[40;31m"
#define GRN "\033[40;32m"

std::tuple<std::string, std::string> StringDiff::getDiffString(std::string const &actual, std::string const &expected) {
    std::string res1 = "", res2 = "";
    auto ne = false;
    auto len = std::max(actual.length(), expected.length());
    for (int i = 0; i < len; ++i) {
        if (i>=actual.length() || i>=expected.length() || actual[i] != expected[i]) {
            if (!ne) {
                res1 += RED;
                res2 += GRN;
                ne = true;
            }
            if (i < actual.length()) {
                res1 += actual[i];
            }
            if (i < expected.length()) {
                res2 += expected[i];
            }
        } else {
            if (ne) {
                res1 += "\033[0m";
                res2 += "\033[0m";
            }
            ne = false;
            if (i < actual.length()) {
                res1 += actual[i];
            }
            if (i < expected.length()) {
                res2 += expected[i];
            }
        }
    }
    return {res1, res2};
}
