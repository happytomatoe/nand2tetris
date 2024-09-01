#include "ConfigParser.h"

#include <iostream>
#include <toml.hpp>

Config ConfigParser::parse_file(const string &file_location) {
    if (std::ifstream file(file_location); file.peek() == std::ifstream::traits_type::eof()) {
        throw std::runtime_error("Config file is empty");
    }
    try {
        auto config = toml::parse(file_location);
        return parse(config);
    } catch (exception &e) {
        cerr << "Exception when trying to parse config file " << file_location << endl;
        throw;
    }
}

const map<string, memory::MemorySegment> segmentsTypes = {
    {"stack", memory::Stack},
    {"local", memory::Local,}, {"arg", memory::Arg}, {"this", memory::This},
    {"that", memory::That}, {"pointer", memory::Pointer}, {"temp", memory::Temp}, {"static", memory::Static}
};

Config ConfigParser::parse_text(const string &config) {
    return parse(toml::parse_str(config));
}

Config ConfigParser::parse(const toml::basic_value<toml::type_config> &config) {
    //TODO: add validation
    const bool clear_stack = toml::find_or<bool>(config, "memory", "stack", "clear", true);
    const bool memory_init = toml::find_or<bool>(config, "memory", "initialize", true);
    map<memory::MemorySegment, memory::Range> memory_segment_min_max_adress =
            memory::default_memory_segment_min_max_adress;
    for (const auto &[segment_string, segment_type]: segmentsTypes) {
        const auto min = toml::find<int>(config, "memory", "segment-addresses", segment_string, "min");
        const auto max = toml::find<int>(config, "memory", "segment-addresses", segment_string, "max");
        memory_segment_min_max_adress[segment_type] = memory::Range{min, max};
    }
    return Config{memory_segment_min_max_adress, clear_stack, memory_init};
}
