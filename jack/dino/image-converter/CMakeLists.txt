cmake_minimum_required(VERSION 3.28)
project(image_converter)
set(CMAKE_CXX_STANDARD 23)



add_library(converter)
target_sources(converter
        PUBLIC
        src/converter/ImageConverter.hpp
        PRIVATE
        src/converter/ImageConverter.cpp
        src/converter/StringUtils.cpp
)
set_target_properties(converter PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(converter PUBLIC src )

find_package(re2)
find_package(cpptrace)
target_link_libraries(converter PUBLIC re2::re2 cpptrace::cpptrace)

add_executable(image_converter src/main.cpp)
target_link_libraries(image_converter PUBLIC converter)


# enable testing functionality

find_package(GTest)
add_executable(tests test/test.cpp)
target_link_libraries(tests PUBLIC gtest::gtest converter)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
