#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <filesystem>
#include "Lexer.h"
#include "Parser.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
#ifdef NDEBUG
    printf("Release configuration!\n");
#else
    printf("Debug configuration!\n");
#endif
    if (argc < 2) {
        std::cerr << "missing input file!\n";
        std::cerr << "usage: assembler <input_file> ...\n";
        return EXIT_FAILURE;
    }
    std::cout << "Reading file: " << argv[1] << "" << std::endl;
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open file. Exiting out...";
        exit(0); //failed to open file
    }
    std::vector<std::string> lines;
    std::string str;
    while (std::getline(file, str)) {
        lines.push_back(str);
    }
    for (int i = 0; i < lines.size(); i++) {
        auto s = Lexer::lex(lines[i]);
        Parser::parse();
    }
    file.close();

    //close file
    return EXIT_SUCCESS;
}
