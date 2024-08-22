

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
        src/assemble/Assembler.h
        src/assemble/Lexer.h
        src/assemble/Parser.h
        src/assemble/exception.h
        PRIVATE
        src/assemble/StringUtils.h
        src/assemble/Lexer.cpp
        src/assemble/Assembler.cpp
        src/assemble/Parser.cpp
)

target_include_directories(assemble PUBLIC src)

find_package(spdlog)
find_package(cpptrace REQUIRED)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(assemble PUBLIC cpptrace::cpptrace spdlog::spdlog OpenMP::OpenMP_CXX)
else()
    target_link_libraries(assemble PUBLIC cpptrace::cpptrace spdlog::spdlog OpenMP::OpenMP_CXX)
endif()



find_package(CLI11 REQUIRED)
add_executable(assembler src/main.cpp)
target_link_libraries(assembler PRIVATE assemble CLI11::CLI11)


#benchmark
find_package(benchmark REQUIRED)
add_executable(benchmark benchmark/benchmark.cpp)
target_link_libraries(benchmark PRIVATE benchmark::benchmark_main)

# testing binary

find_package(GTest REQUIRED)
add_executable(tests
        test/assemble_test.cpp
        test/parser_test.cpp
        test/lexer_test.cpp
        test/StringDiff.cpp
        test/StringDiff.h
        test/Utils.h)
target_link_libraries(tests PRIVATE assemble gtest::gtest)

# enable testing functionality
enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
