//
// Created by babkamen on 8/18/24.
//
#ifndef UTILS_H
#define UTILS_H
#include <string>


class Utils {
public:
    template<typename Range, typename Value = typename Range::value_type>
    static std::string Join(Range const &elements, const char *const delimiter) {
        std::stringstream ss;
        for (auto const &element: elements) {
            ss << element << delimiter;
        }
        return ss.str();
    }

    /*! note: imput is assumed to not contain NUL characters
     */
    template<typename Input, typename Output, typename Value = typename Output::value_type>
    void Split(char delimiter, Output &output, Input const &input) {
        using namespace std;
        for (auto cur = begin(input), beg = cur; ; ++cur) {
            if (cur == end(input) || *cur == delimiter || !*cur) {
                output.insert(output.end(), Value(beg, cur));
                if (cur == end(input) || !*cur)
                    break;
                else
                    beg = next(cur);
            }
        }
    }
};


#endif //UTILS_H
