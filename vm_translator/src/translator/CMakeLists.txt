add_library(translator)
target_sources(translator
        PUBLIC
        Translator.h
        PRIVATE
        Translator.cpp
        Lexer.h
        Lexer.cpp
)

find_package(cpptrace REQUIRED)
target_include_directories(translator PRIVATE translator)
target_link_libraries(translator cpptrace::cpptrace)