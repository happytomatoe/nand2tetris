#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <converter/ImageConverter.hpp>
#include "dino.cpp"
#include "converter/StringUtils.h"

#include <cpptrace/cpptrace.hpp>
using namespace std;
TEST(ConverterTest, ReadTest) {
    auto pixels = ImageConverter::read(text);
    cout << "Read" << endl;
    StringUtils::print(pixels);
    auto normalized = ImageConverter::normalize(pixels);
    cout << "Normalized" << endl;
    StringUtils::print(normalized);
    auto res = ImageConverter::convert(normalized);
    cout << res;
}

TEST(ConverterTest, StringUtils) {
    ASSERT_EQ(StringUtils::snake_case_to_camel_case("dino_with_white"), "DinoWithWhite");
}
