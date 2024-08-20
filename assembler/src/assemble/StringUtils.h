#include <string>

#ifndef UTILS_H
#define UTILS_H


class StringUtils {
public:
    template<typename T>
    static std::string to_string(const T &value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }
};


#endif //UTILS_H
