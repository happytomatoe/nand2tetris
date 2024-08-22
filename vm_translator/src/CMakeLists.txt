add_subdirectory(translator)

find_package(CLI11 REQUIRED)
add_executable(vm main.cpp)
target_link_libraries(vm INTERFACE CLI11::CLI11 translator)

