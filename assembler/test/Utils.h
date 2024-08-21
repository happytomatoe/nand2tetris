//
// Created by babkamen on 8/18/24.
//
#ifndef UTILS_H
#define UTILS_H
#include <string>


class Utils {
public:
    template<typename Range>
    static std::string join(Range const &elements, const char *const delimiter) {
        std::stringstream ss;
        for (auto const &element: elements) {
            ss << element << delimiter;
        }
        return ss.str();
    }

    template<typename T>
    static std::string toString(T &obj) {
        std::stringstream ss;
        ss << obj;
        return ss.str();
    }
};


#endif //UTILS_H
