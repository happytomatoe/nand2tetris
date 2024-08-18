

cmake_minimum_required(VERSION 3.28)
project(assembler)

#find_package(CATCH2 REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

message("Building with CMake version: ${CMAKE_VERSION}")

# example library
add_library(assemble)
target_sources(assemble
        PUBLIC
        src/assemble.h
        PRIVATE
        src/assemble.cpp
)
target_include_directories(assemble PUBLIC src)
add_executable(assembler src/main.cpp)
target_link_libraries(assembler PRIVATE assemble)


# testing binary

#find_package(Catch2 REQUIRED)
find_package(GTest REQUIRED)
add_executable(tests test/test.cpp)
#target_link_libraries(tests PRIVATE assemble Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE assemble gtest::gtest)

# enable testing functionality
enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)

## define tests
#add_test(
#        NAME tests
#        COMMAND $<TARGET_FILE:tests>
#)
