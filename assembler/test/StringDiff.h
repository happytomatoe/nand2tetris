//
// Created by babkamen on 8/18/24.
//

#ifndef STRINGDIFF_H
#define STRINGDIFF_H
#include <string>


class StringDiff {
public:
    static std::tuple<std::string, std::string> getDiffString(std::string const &actual, std::string const &expected);
};


#endif //STRINGDIFF_H
