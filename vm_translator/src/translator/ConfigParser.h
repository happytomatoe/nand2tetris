#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <ostream>
#include <types.hpp>

#include "MemorySegment.h"


struct Config {
    const map<memory::MemorySegment, memory::Range> memory_segment_min_max_adress;
    bool clear_stack;
    bool memory_init;

    friend std::ostream &operator<<(std::ostream &os, const Config &obj) {
        os << "Config(" << endl
                << "clear_stack: " << obj.clear_stack << endl
                << "memory_init: " << obj.memory_init << endl;
        for (const auto &[segment, range]: obj.memory_segment_min_max_adress) {
            os << toString(segment) << ":( " << range << " )" << endl;
        }
        os << ")";
        return os;
    }
};

class ConfigParser {
public:
    static Config parse_file(const string &file_location);

    static Config parse(const toml::basic_value<toml::type_config> &config);

    static Config parse_text(const string &config);
};


#endif //CONFIGPARSER_H
