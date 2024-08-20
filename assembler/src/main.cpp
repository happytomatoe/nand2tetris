#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <filesystem>

#include "assemble//Assembler.h"
#include "assemble/Lexer.h"
#include "assemble/Parser.h"
#include <CLI/CLI.hpp>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    CLI::App app{"Hack assembler"};
    argv = app.ensure_utf8(argv);

    std::string input_file, output_file;
    app.add_option("input", input_file, "input file path")->required();
    app.add_option("-o", output_file, "output file name");

    CLI11_PARSE(app, argc, argv);
    auto res = Assembler::assemble(argv[1]);

    if(output_file.empty()) {
        cout << res;
    }else {
        ofstream out_file(output_file);
        out_file << res;
    }

    //close file
    return EXIT_SUCCESS;
}
