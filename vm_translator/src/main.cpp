#include <iostream>
#include <fstream>
#include <string>

#include "translator/Translator.h"
#include "CLI/CLI.hpp"
using namespace std;


int main(int argc, char *argv[]) {
    CLI::App app{"Hack assembler"};
    // CLI::App app{"Hack assembler"};
    // argv = app.ensure_utf8(argv);
    //
    // string input_file, output_file;
    // app.add_option("input", input_file, "input file path")->required();
    // app.add_option("-o", output_file, "output file name");
    //
    // CLI11_PARSE(app, argc, argv);
    // auto res = Translator().translate(input_file);
    //
    // if (output_file.empty()) {
    //     cout << res;
    // } else {
    //     ofstream out_file(output_file);
    //     out_file << res;
    // }

    //close file
    return EXIT_SUCCESS;
}
