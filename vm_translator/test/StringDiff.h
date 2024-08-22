#ifndef STRINGDIFF_H
#define STRINGDIFF_H
#include <string>

#include <iostream>
#include <memory>
#include <string>

#define RED "\033[40;31m"
#define GRN "\033[40;32m"
using namespace std;

class StringDiff {
public:
    static std::tuple<std::string, std::string> get_diff_single_line_strings(
        std::string const &actual, std::string const &expected) {
        string res1 = "", res2 = "";
        auto ne = false;
        auto len = max(actual.length(), expected.length());
        for (int i = 0; i < len; ++i) {
            if (i >= actual.length() || i >= expected.length() || actual[i] != expected[i]) {
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

    static std::string get_diff_multi_line_strings(
        const std::string &actual, const std::string &expected, bool anotate) {
        string res = "", str1 = "", str2 = "";
        int i = 0, line_number = 0;
        int j = 0;
        while (i < actual.length()) {
            str1 += actual[i];
            if (actual[i] == '\n') {
                while (j < expected.length() && expected[j] != '\n') {
                    str2 += expected[j];
                    j++;
                }
                str2 += '\n';
                j++;
                if (str1 != str2) {
                    auto [str1Colored, str2Colored] = get_diff_single_line_strings(str1, str2);
                    if (anotate) {
                        res += to_string(line_number) + ":\t\t";
                    }
                    res += str1Colored;
                    if (anotate) {
                        res += to_string(line_number) + ":\t\t";
                    }
                    res += str2Colored;
                } else {
                    if (anotate) {
                        res += to_string(line_number) + ":\t\t";
                    }
                    res += str1;
                }
                line_number++;
                str1 = "", str2 = "";
            }
            ++i;
        }
        return res;
    }
};


#endif //STRINGDIFF_H
