#include <iostream>
#include <fstream>
#include <string>

#include "translator/Translator.h"
#include "CLI/CLI.hpp"
using namespace std;


int main(int argc, char *argv[]) {
    CLI::App app{"Hack vm translator"};
    argv = app.ensure_utf8(argv);

    string input_file, output_file;
    optional<string> config_file;
    app.add_option("input", input_file, "input file path")->required();
    app.add_option("-o", output_file, "output file name");
    app.add_option("-c,-config-file-location", config_file, "Config file location");

    CLI11_PARSE(app, argc, argv);
    string res;
    if (config_file.has_value()) {
        res = Translator().translate(input_file, ConfigParser::parse_file(config_file.value()));
    } else {
        res = Translator().translate(input_file);
    }

    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        out_file << res;
    }

    //close file
    return EXIT_SUCCESS;
}
