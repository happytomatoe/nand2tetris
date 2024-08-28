#pragma once
#include <string>

#include <iostream>
#include <memory>
#include <string>
#include <translator/Token.h>

#include "Utils.h"
#include "translator/exception.h"

#define RED "\033[40;31m"
#define GRN "\033[40;32m"
#define END "\033[0m"

namespace token {
    struct Token;
}

using namespace std;

class StringDiff {
public:
    static tuple<string, string> diff(vector<token::Token> actual, vector<token::Token> expected) {
        ostringstream actualRes, expectedRes;
        if (actual.size() >= expected.size()) {
            for (auto i = 0; i < actual.size(); ++i) {
                if (i > 0) {
                    actualRes << ", ";
                    if (i < expected.size()) {
                        expectedRes << ", ";
                    }
                }
                auto actualToken = actual[i];
                if (i < expected.size()) {
                    auto expectedToken = expected[i];
                    compare(actualToken, expectedToken, actualRes, expectedRes);
                } else {
                    compare(actualToken, nullopt, actualRes, expectedRes);
                }
            }
        } else if (actual.size() < expected.size()) {
            for (auto i = 0; i < expected.size(); ++i) {
                if (i > 0) {
                    expectedRes << ", ";
                    if (i < actual.size()) {
                        actualRes << ", ";
                    }
                }
                auto expectedToken = expected[i];
                if (i < actual.size()) {
                    auto actualToken = actual[i];
                    compare(actualToken, expectedToken, actualRes, expectedRes);
                } else {
                    compare(nullopt, expectedToken, actualRes, expectedRes);
                }
            }
        }
        return {actualRes.str(), expectedRes.str()};
    }

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
                return count_if(s.begin(), s.end(), [](char c) {
                    return !isspace(c) || c == '\n' || c == '\t';
                }) == 0;
            });
        return res;
    }


    static string get_diff_multi_line_strings_in_reverse_order(
        const std::string &actual, const std::string &expected, int rightPad = 10) {
        auto actualLines = remove_if_line_is_empty(split(actual, '\n'));
        auto expectedLines = remove_if_line_is_empty(split(expected, '\n'));
        if (actualLines.size() > expectedLines.size()) {
            auto lenDiff = actualLines.size() - expectedLines.size();
            for (int i = 0; i < lenDiff; ++i) {
                expectedLines.insert(expectedLines.begin(), "");
            }
        } else if (actualLines.size() < expectedLines.size()) {
            for (int i = 0; i < expectedLines.size() - actualLines.size(); ++i) {
                //TODO: should this be changed as we can have 2 cases - lines are not enough in the front or in the back
                actualLines.emplace_back("");
            }
        }
        string res;
        for (int i = actualLines.size() - 1; i >= 0; --i) {
            auto actualLine = actualLines[i];
            auto expectedLine = expectedLines[i];
            if (actualLine != expectedLine) {
                auto [actualLineColored, expectedLineColored] = get_diff_single_line_strings(
                    actualLine, expectedLine);

                if (!expectedLine.empty()) {
                    res.insert(0, expectedLineColored);
                }
                if (!actualLine.empty()) {
                    auto padded = Utils::rightPadTo(actualLineColored, rightPad, ' ', true);
                    res.insert(0, padded);
                }
                res.insert(0, "\n");
            } else {
                res.insert(0, "\n" + actualLine);
            }
        }
        return res;
    }

private:
    static void compare(const optional<token::Token> &maybeActualToken,
                        const optional<token::Token> &maybeExpectedToken,
                        ostringstream &actualRes,
                        ostringstream &expectedRes) {
        if (!maybeActualToken.has_value() && !maybeExpectedToken.has_value()) {
            throw cpptrace::logic_error("Both tokens are empty");
        }

        if (maybeActualToken.has_value() && !maybeExpectedToken.has_value()) {
            actualRes << RED << maybeActualToken.value() << END;
            return;
        } else if (maybeExpectedToken.has_value() && !maybeActualToken.has_value()) {
            expectedRes << GRN << maybeExpectedToken.value() << END;
            return;
        }
        auto actualToken = maybeActualToken.value();
        auto expectedToken = maybeExpectedToken.value();
        if (actualToken != expectedToken) {
            actualRes << "Token(type: ";
            expectedRes << "Token(type: ";
            if (actualToken.type != expectedToken.type) {
                actualRes << RED << toString(actualToken.type) << END;
                expectedRes << GRN << toString(expectedToken.type) << END;
            } else {
                actualRes << toString(actualToken.type) << END;
                expectedRes << toString(expectedToken.type) << END;
            }

            actualRes << ", category: ";
            expectedRes << ", category: ";
            if (actualToken.category != expectedToken.category) {
                actualRes << RED << toString(actualToken.category) << END;
                expectedRes << GRN << toString(expectedToken.category) << END;
            } else {
                actualRes << toString(actualToken.category) << END;
                expectedRes << toString(expectedToken.category) << END;
            }
            actualRes << ", number: ";
            expectedRes << ", number: ";
            if (actualToken.number != expectedToken.number) {
                actualRes << RED << actualToken.number << END;
                expectedRes << GRN << expectedToken.number << END;
            } else {
                actualRes << actualToken.number << END;
                expectedRes << expectedToken.number << END;
            }

            actualRes << ", functionArgumentCount: ";
            expectedRes << ", functionArgumentCount: ";
            if (actualToken.functionArgumentCount != expectedToken.functionArgumentCount) {
                actualRes << RED << actualToken.functionArgumentCount << END;
                expectedRes << GRN << expectedToken.functionArgumentCount << END;
            } else {
                actualRes << actualToken.functionArgumentCount << END;
                expectedRes << expectedToken.functionArgumentCount << END;
            }

            actualRes << ", label: ";
            expectedRes << ", label: ";
            if (actualToken.label != expectedToken.label) {
                actualRes << RED << actualToken.label << END;
                expectedRes << GRN << expectedToken.label << END;
            } else {
                actualRes << actualToken.label << END;
                expectedRes << expectedToken.label << END;
            }

            actualRes << ", functionName: ";
            expectedRes << ", functionName: ";
            if (actualToken.functionName != expectedToken.functionName) {
                actualRes << RED << actualToken.functionName << END;
                expectedRes << GRN << expectedToken.functionName << END;
            } else {
                actualRes << actualToken.functionName << END;
                expectedRes << expectedToken.functionName << END;
            }
            actualRes << ")";
            expectedRes << ")";
        } else {
            actualRes << actualToken;
            expectedRes << expectedToken;
        }
    }
};
