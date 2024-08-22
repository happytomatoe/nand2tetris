cmake_minimum_required(VERSION 3.28)
project(vm)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(src)

# enable testing functionality

find_package(GTest)
add_executable(tests src/main.cpp
        test/lexer_test.cpp)
target_link_libraries(tests gtest::gtest)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
