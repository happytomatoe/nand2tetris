cmake_minimum_required(VERSION 3.28)
project(jack-image-converter)
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

find_package(lodepng REQUIRED)
find_package(CLI11 REQUIRED)
find_package(clip REQUIRED)
find_package(cpptrace REQUIRED)
target_link_libraries(converter PUBLIC cpptrace::cpptrace clip::clip CLI11::CLI11 lodepng::lodepng)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC converter)

install(TARGETS ${PROJECT_NAME} DESTINATION ~/bin)

# enable testing functionality


#test resources
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#tests
find_package(GTest)
add_executable(tests test/converter_test.cpp)
target_link_libraries(tests PUBLIC gtest::gtest converter)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
