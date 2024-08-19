

cmake_minimum_required(VERSION 3.28)
project(assembler)

#find_package(CATCH2 REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

message("Building with CMake version: ${CMAKE_VERSION}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -fsanitize=address")

# example library
add_library(assemble)
target_sources(assemble
        PUBLIC
        src/assemble.h
        src/Lexer.h
        src/Parser.h
        PRIVATE
        src/Lexer.cpp
        src/assemble.cpp
        src/Parser.cpp
)
target_include_directories(assemble PUBLIC src)
find_package(cpptrace REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(assemble PUBLIC cpptrace::cpptrace spdlog::spdlog)

add_executable(assembler src/main.cpp)
target_link_libraries(assembler PRIVATE assemble)


# testing binary

#find_package(Catch2 REQUIRED)
find_package(GTest REQUIRED)
add_executable(tests test/assemble_test.cpp test/parser_test.cpp test/lexer_test.cpp
        test/StringDiff.cpp
        test/StringDiff.h
        test/Utils.h)
#target_link_libraries(tests PRIVATE assemble Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE assemble gtest::gtest)

# enable testing functionality
enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
