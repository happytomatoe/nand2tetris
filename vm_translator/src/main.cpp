#include <csignal>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "translator/Translator.h"
#include <cpptrace/cpptrace.hpp>
#include "CLI/CLI.hpp"
using namespace std;

const auto assembler_extension = ".asm";
const string vm_file_extension = ".vm";

void process_file(const string &input_file_or_dir, const string &output_file, const Config &config) {
    const string res = Translator().translate(input_file_or_dir, config);
    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        out_file << res;
        out_file.close();
    }
}


void handle_dir(const filesystem::path &input_dir, const Config &config) {
    vector<filesystem::path> input_files = {};

    for (const auto &entry: filesystem::directory_iterator(input_dir)) {
        if (is_regular_file(entry) && entry.path().extension() == vm_file_extension) {
            input_files.push_back(entry.path());
        }
    }
    if (input_files.empty()) {
        throw std::runtime_error(format("No {} files found in the input directory", vm_file_extension));
    }
    bool has_sys_vm = ranges::find_if(input_files, [](const filesystem::path &p) {
        return p.filename() == "Sys.vm";
    }) != input_files.end();
    if (has_sys_vm) {
        cout << "Input dir has Sys.vm file" << endl;
    }
    for (auto &input_file: input_files) {
        auto output_file = input_file;
        output_file.replace_extension(assembler_extension);
        cout << "Input file " << input_file << endl;
        cout << "Output file " << output_file << endl;
        if (has_sys_vm) {
            auto filename = input_file.filename().string();
            cout << "Filename " << filename << endl;
            if (filename == "Sys.vm") {
                process_file(input_file.string(), output_file, config);
            } else {
                auto config_dup = config;
                config_dup.memory_init = false;
                process_file(input_file.string(), output_file, config_dup);
            }
        } else {
            process_file(input_file.string(), output_file, config);
        }
    }
}

int main(int argc, char *argv[]) {
    cpptrace::register_terminate_handler();
    CLI::App app{"Hack vm translator"};
    argv = app.ensure_utf8(argv);
    string input_file_or_dir;
    optional<string> config_file;
    app.add_option("input", input_file_or_dir, "input file directory path")
            ->check(CLI::ExistingPath)->required();
    app.add_option("-c,--config-file-location", config_file, "Config file location");
    CLI11_PARSE(app, argc, argv);
    //check if there are .vm files inside input directory
    const filesystem::path path(input_file_or_dir);
    error_code ec; // For using the non-throwing overloads of functions below.
    auto config = config_file.has_value() ? ConfigParser::parse_file(config_file.value()) : Translator::default_config;
    if (is_directory(path, ec)) {
        handle_dir(path, config);
    }
    if (ec) // Optional handling of possible errors.
    {
        cerr << "Error in is_directory: " << ec.message();
        return EXIT_FAILURE;
    }
    if (is_regular_file(path, ec)) {
        if (input_file_or_dir.find_last_of('.') == string::npos) {
            throw runtime_error("Input file has no extension");
        }
        const auto output_file = input_file_or_dir.substr(0, input_file_or_dir.find_last_of('.')) + assembler_extension;
        process_file(input_file_or_dir, output_file, config);
    }
    if (ec) {
        cerr << "Error in is_regular_file: " << ec.message();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
