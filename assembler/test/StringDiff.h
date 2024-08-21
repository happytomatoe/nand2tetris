//
// Created by babkamen on 8/18/24.
//

#ifndef STRINGDIFF_H
#define STRINGDIFF_H
#include <string>


class StringDiff {
public:
    static std::tuple<std::string, std::string> get_diff_single_line_strings(
        std::string const &actual, std::string const &expected);

    static std::string get_diff_multi_line_strings(
        const std::string &string, const std::string &expected, bool anotate);
};


#endif //STRINGDIFF_H
