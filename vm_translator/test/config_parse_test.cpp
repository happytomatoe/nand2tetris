#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "translator/ConfigParser.h"

TEST(ConfigParseTest, BasicTest) {

    auto text = R"(
        [memory]
        stack.clear = true
        initialize = true
        [memory.segment-addresses]
        stack = { min = 256, max = 299 }
        local = { min = 256, max = 399 }
        arg = { min = 256, max = 499 }
        this = { min = 3000, max = 3009 }
        that = { min = 3010, max = 4000 }
        pointer = { min = 3, max = 4 }
        temp = { min = 5, max = 12 }
        static = { min = 16, max = 255 }
    )";
    auto config = ConfigParser::parse_text(text);
    cout<< config << endl;
    ASSERT_EQ(config.clear_stack, true);
}