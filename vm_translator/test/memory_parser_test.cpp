#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "translator/MemorySegmentsParser.h"


TEST(MemoryParser, BasicTest) {
    auto text = R"(
{"stack": [1, 2], "local": [3, 4], "arg": [5, 6], "this": [7, 8], "that": [9, 10], "pointer": [11, 12],  "temp": [12, 13], "static": [14, 15]}
    )";
    auto config = MemorySegmentsParser::parse(text);
    auto expected = map<memory::MemorySegment, memory::Range>{
        {memory::Stack, memory::Range{1, 2}},
        {memory::Local, memory::Range{3, 4}},
        {memory::Arg, memory::Range{5, 6}},
        {memory::This, memory::Range{7, 8}},
        {memory::That, memory::Range{9, 10}},
        {memory::Pointer, memory::Range{11, 12}},
        {memory::Temp, memory::Range{12, 13}},
        {memory::Static, memory::Range{14, 15}}
    };
    ASSERT_EQ(config, expected);
}
