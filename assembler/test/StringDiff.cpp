#include "StringDiff.h"

#include <iostream>
#include <string>

#define RED "\033[40;31m"
#define GRN "\033[40;32m"

std::tuple<std::string, std::string> StringDiff::get_diff_string(std::string const& actual, std::string const &expected) {
    std::string res1 = "", res2 = "";
    auto ne = false;
    for (int i = 0; i < actual.length(); ++i) {
        if (actual[i] != expected[i]) {
            if (!ne) {
                res1 += RED;
                res2 += GRN;
                ne = true;
            }
            res1 += actual[i];
            res2 += expected[i];
        } else {
            if (ne) {
                res1 += "\033[0m";
                res2 += "\033[0m";
            }
            ne = false;
            res1 += actual[i];
            res2 += expected[i];
        }
    }
    return {res1, res2};
}
