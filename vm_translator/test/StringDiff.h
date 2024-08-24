#pragma once
#include <string>

#include <iostream>
#include <memory>
#include <string>

#include "Utils.h"
#include "translator/exception.h"

#define RED "\033[40;31m"
#define GRN "\033[40;32m"
#define END "\033[0m"
using namespace std;

class StringDiff {
public:
    static std::tuple<std::string, std::string> get_diff_single_line_strings(
        std::string const &actual, std::string const &expected) {
        string actualRes, expectedRes;
        auto ne = false;
        auto len = max(actual.length(), expected.length());
        for (int i = 0; i < len; ++i) {
            if (actual[i] == '\n' || expected[i] == '\n') {
                throw cpptrace::exception_with_message("Found newline in diff single string");
            }
            if (i >= actual.length() || i >= expected.length() || actual[i] != expected[i]) {
                if (!ne) {
                    actualRes += RED;
                    expectedRes += GRN;
                    ne = true;
                }
                if (i < actual.length()) {
                    actualRes += actual[i];
                }
                if (i < expected.length()) {
                    expectedRes += expected[i];
                }
            } else {
                if (ne) {
                    actualRes += END;
                    expectedRes += END;
                }
                ne = false;
                if (i < actual.length()) {
                    actualRes += actual[i];
                }
                if (i < expected.length()) {
                    expectedRes += expected[i];
                }
            }
        }
        if (ne) {
            actualRes += END;
            expectedRes += END;
        }
        return {actualRes, expectedRes};
    }

    static std::string get_diff_multi_line_strings(
        const std::string &actual, const std::string &expected, bool anotate) {
        string res, actualLine, expectedLine;
        int i = 0, line_number = 0;
        int j = 0;
        while (i < actual.length()) {
            actualLine += actual[i];
            if (actual[i] == '\n') {
                while (j < expected.length() && expected[j] != '\n') {
                    expectedLine += expected[j];
                    j++;
                }
                expectedLine += '\n';
                j++;
                if (actualLine != expectedLine) {
                    auto [actualLineColored, expectedLineColored] = get_diff_single_line_strings(
                        actualLine, expectedLine);
                    if (anotate) {
                        res += to_string(line_number) + ":\t\t";
                    }
                    res += actualLineColored;
                    if (anotate) {
                        res += to_string(line_number) + ":\t\t";
                    }
                    res += expectedLineColored;
                } else {
                    if (anotate) {
                        res += to_string(line_number) + ":\t\t";
                    }
                    res += actualLine;
                }
                line_number++;
                actualLine = "", expectedLine = "";
            }
            ++i;
        }
        return res;
    }


    static string remove_char(const string &s, const char c) {
        string res;
        for (auto ch: s) {
            if (ch != c) {
                res += ch;
            }
        }
        return res;
    }

    static vector<string> split(const std::string &str, char delimeter) {
        auto result = std::vector<std::string>{};
        auto ss = std::stringstream{str};

        for (std::string line; std::getline(ss, line, delimeter);)
            result.push_back(line);

        return result;
    }

    static vector<string> remove_if_line_is_empty(const vector<string> &s) {
        vector<string> res;
        std::remove_copy_if(
            s.begin(),
            s.end(),
            std::back_inserter(res),
            [](const string &s) {
                return count_if(s.begin(), s.end(), [](char c) { return !isspace(c) || c == '\n' || c == '\t'; }) == 0;
            });
        return res;
    }


    static string get_diff_multi_line_strings_in_reverse_order(
        const std::string &actual, const std::string &expected) {
        auto actualLines = remove_if_line_is_empty(split(actual, '\n'));
        auto expectedLines = remove_if_line_is_empty(split(expected, '\n'));
        if (actualLines.size() > expectedLines.size()) {
            for (int i = 0; i < actualLines.size() - expectedLines.size(); ++i) {
                expectedLines.emplace_back("");
            }
        } else if (actualLines.size() < expectedLines.size()) {
            for (int i = 0; i < expectedLines.size() - actualLines.size(); ++i) {
                actualLines.emplace_back("");
            }
        }
        string res;
        for (int i = actualLines.size() - 1; i >= 0; --i) {
            auto actualLine = actualLines[i];
            auto expectedLine = expectedLines[i];
            if (actualLine != expectedLine) {
                auto [actualLineColored, expectedLineColored] = get_diff_single_line_strings(actualLine, expectedLine);
                if (!actualLine.empty()) {
                    res.insert(0, Utils::rightPadTo(actualLineColored, 10));
                }
                if (!expectedLine.empty()) {
                    res.insert(0, expectedLineColored);
                }
                res.insert(0, "\n");
            } else {
                res.insert(0, "\n" + actualLine);
            }
        }
        return res;
    }
};

