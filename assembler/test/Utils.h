//
// Created by babkamen on 8/18/24.
//
#ifndef UTILS_H
#define UTILS_H
#include <string>


class Utils {
public:
    template<typename Range>
    static std::string Join(Range const &elements, const char *const delimiter) {
        std::stringstream ss;
        for (auto const &element: elements) {
            ss << element << delimiter;
        }
        return ss.str();
    }
};


#endif //UTILS_H
