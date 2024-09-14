#pragma once
#include <ostream>
#include "MemorySegment.h"

namespace translator {


struct Config {
    const map<memory::MemorySegment, memory::Range> memory_segment_min_max_adress;
    bool clear_stack,  memory_init, enable_comments;


    friend std::ostream &operator<<(std::ostream &os, const Config &obj) {
        os << "Config(" << endl
                << "clear_stack: " << obj.clear_stack << endl
                << "memory_init: " << obj.memory_init << endl
                << "disable_comments: " << obj.enable_comments << endl;
        for (const auto &[segment, range]: obj.memory_segment_min_max_adress) {
            os << toString(segment) << ":( " << range << " )" << endl;
        }
        os << ")";
        return os;
    }
};

}