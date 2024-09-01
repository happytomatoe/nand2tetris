#pragma once
#include <string>
#include <regex>

using namespace std;

class Utils {
public:
    template<typename Range>
    static string join(Range const &elements, const char *const delimiter) {
        stringstream ss;
        for (auto const &element: elements) {
            ss << element << delimiter;
        }
        return ss.str();
    }

    template<typename T>
    static string toString(T &obj) {
        stringstream ss;
        ss << obj;
        return ss.str();
    }

    static string preprocess(const string &str) {
        string res;
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] == '/') {
                if (i + 1 < str.length() && str[i + 1] == '/') {
                    while (i < str.length() && str[i] != '\n') {
                        i++;
                    }
                }
            }
            if (str[i] == '\n' && (res.empty() || res.back() == '\n')) {
                continue;
            }
            if (str[i] != ' ') {
                res += str[i];
            }
        }
        return res;
    }

    static bool replace(std::string &str, const std::string &from, const std::string &to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    static string leftPadTo(std::string &str, const size_t num, const char paddingChar = ' ') {
        auto str2 = str;
        if (num > str2.size())
            str2.insert(0, num - str2.size(), paddingChar);
        return str2;
    }

    static string rightPadTo(std::string &str, const size_t num, const char paddingChar = ' ', bool excludeColorCodes =
                                     false) {
        auto str2 = str;
        int count = str2.size();
        if (excludeColorCodes) {
            regex color("\033[[;\\d]+m");
            auto s = regex_replace(str2, color, "");
            count = s.size();
        }
        if (num > count)
            str2.insert(str2.end(), num - count, paddingChar);
        return str2;
    }
};
