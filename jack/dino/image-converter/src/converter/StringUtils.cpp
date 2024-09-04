#include <sstream>
#include <string>
#include <vector>
using namespace std;

class StringUtils {
public:
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
        const auto lines = split(s, '\n');
        string res;
        for (auto &line: lines) {
            int delimiterPos = 0;
            for (int i = 0; i < line.length(); ++i) {
                auto c = line[i];
                if (c == ' ' || c == '\t') {
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

    static string snake_case_to_camel_case(const string &str) {
        // Empty String
        string result;


        for (int i = 0; i < str.length(); i++) {
            char ch = str[i];

            //delete _ and make upper next char
            if (i == 0) {
                result.append(1, static_cast<char>(toupper(ch)));
            } else if (ch == '_') {
            } else if (i > 0 && str[i - 1] == '_') {
                if (i < str.length() - 1) {
                    result.append(1, toupper(ch));
                }
            } else {
                result.append(1, static_cast<char>(tolower(ch)));
            }
        }

        // return the result
        return result;
    }
};
