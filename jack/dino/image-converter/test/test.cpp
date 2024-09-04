#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <converter/ImageConverter.hpp>
#include "dino.cpp"
#include "converter/StringUtils.cpp"

#include <cpptrace/cpptrace.hpp>
using namespace std;
TEST(ConverterTest, ReadTest) {
    auto im = ImageConverter::read(text);
    ASSERT_EQ(im.rows, 43);
    ASSERT_EQ(im.cols, round(double(40)/16));
    cout << im;
    ASSERT_EQ(im.pixels.size(), im.rows);
    ASSERT_EQ(im.pixels[0].size(), 1);
    ASSERT_EQ(im.file_name, "dino_with_white");
}

TEST(ConverterTest, StringUtils) {
    ASSERT_EQ(StringUtils::snake_case_to_camel_case("dino_with_white"), "DinoWithWhite");
}


void sigsegv_handler(int sig) {
    cpptrace::generate_trace().print();
    exit(1);
}


int main(int argc, char **argv) {
    signal(SIGSEGV, sigsegv_handler);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
