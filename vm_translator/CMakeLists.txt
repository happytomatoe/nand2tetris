cmake_minimum_required(VERSION 3.28)
project(vm_translator)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)


#add_subdirectory(translator)

add_library(translator)
target_sources(translator
        PUBLIC
        src/translator/exception.h
        src/translator/Translator.h
        src/translator/Lexer.h
        src/translator/Validator.h
        src/translator/MemorySegment.h
        src/translator/assembly.h
        src/translator/ConfigParser.h
        PRIVATE
        src/translator/StringUtils.h
        src/translator/Token.cpp
        src/translator/Translator.cpp
        src/translator/Lexer.cpp
        src/translator/ConfigParser.cpp
)
find_package(toml11 REQUIRED)
find_package(cpptrace REQUIRED)
target_include_directories(translator PUBLIC src)
target_link_libraries(translator PUBLIC cpptrace::cpptrace toml11::toml11)


find_package(CLI11 REQUIRED)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC CLI11::CLI11 translator)

# enable testing functionality

find_package(GTest)
add_executable(tests test/lexer_test.cpp
        test/validator_test.cpp
        test/translator_test.cpp
        test/translator_file_basic_test.cpp
        test/translator_file_advanced_test.cpp
        test/config_parse_test.cpp)
target_link_libraries(tests PUBLIC gtest::gtest translator)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)

