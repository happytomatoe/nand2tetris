#pragma once
#include <string>
#include <vector>
#include <sstream>
using namespace std;

namespace translator {
    static vector<string> split(const std::string &str, char delimeter) {
        auto result = vector<std::string>{};
        std::stringstream ss(str);
        std::string line;
        while (std::getline(ss, line, delimeter)) {
            result.push_back(line);
        }
        return result;
    }

    static std::string stripMargin(const string &s) {
        auto lines = split(s, '\n');
        string res;
        for (auto &line: lines) {
            int delimiterPos = 0;
            for (int i = 0; i < line.length(); ++i) {
                auto c = line[i];
                if (c == ' ') {
                    continue;
                } else if (c == '|') {
                    delimiterPos = i + 1;
                    break;
                } else {
                    break;
                }
            }
            res += line.substr(delimiterPos) + "\n";
        }
        return res;
    }
}
