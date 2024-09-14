#pragma once
#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "MemorySegment.h"
using namespace std;
using json = nlohmann::json;

class MemorySegmentsParser {
public:
    const static map<string, memory::MemorySegment> segmentsTypes;

    static map<memory::MemorySegment, memory::Range> parse(const std::string &text) {
        json data = json::parse(text);
        //TODO: add validation
        //{"stack": [256, 299], "local": [256, 399], "arg": [256, 499], "this": [3000, 3009], "that": [3010, 4000], "pointer": [3, 4], "temp": [5, 12], "static": [16, 255]}
        map<memory::MemorySegment, memory::Range> memory_segment_min_max_adress =
                memory::default_memory_segment_min_max_adress;
        for (const auto &[segment_string, segment_type]: segmentsTypes) {
            if (data.contains(segment_string)) {
                memory_segment_min_max_adress[segment_type] = memory::Range{
                    data[segment_string][0], data[segment_string][1]
                };
            }
        }
        return memory_segment_min_max_adress;
    }
};

// Definition
const map<string, memory::MemorySegment> MemorySegmentsParser::segmentsTypes = {
    {"stack", memory::Stack},
    {"local", memory::Local,}, {"arg", memory::Arg}, {"this", memory::This},
    {"that", memory::That}, {"pointer", memory::Pointer}, {"temp", memory::Temp}, {"static", memory::Static}
};
