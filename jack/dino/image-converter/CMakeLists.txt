cmake_minimum_required(VERSION 3.28)
project(nand2tetris-image-converter)
set(CMAKE_CXX_STANDARD 23)



add_library(converter)
target_sources(converter
        PUBLIC
        src/converter/ImageConverter.hpp
        src/converter/StringUtils.h
        PRIVATE
        src/converter/ImageConverter.cpp
)
set_target_properties(converter PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(converter PUBLIC src )

find_package(CLI11)
find_package(clip REQUIRED)
find_package(re2 REQUIRED)
find_package(cpptrace REQUIRED)
target_link_libraries(converter PUBLIC re2::re2 cpptrace::cpptrace clip::clip CLI11::CLI11)

add_executable(nand2tetris-image-converter src/main.cpp)
target_link_libraries(nand2tetris-image-converter PUBLIC converter)

install(TARGETS nand2tetris-image-converter DESTINATION ~/bin)

# enable testing functionality

find_package(GTest)
add_executable(tests test/test.cpp)
target_link_libraries(tests PUBLIC gtest::gtest converter)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
