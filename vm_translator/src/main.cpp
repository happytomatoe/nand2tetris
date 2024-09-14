#include <csignal>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "translator/Translator.h"
#include <cpptrace/cpptrace.hpp>
#include <fstream>
#include "StringUtils.h"
#include "CLI/CLI.hpp"
#include "translator/MemorySegmentsParser.h"
using namespace std;

constexpr auto assembler_extension = ".asm";
constexpr string vm_file_extension = ".vm";
constexpr string lined_file_name = "Linked";
constexpr auto sys_vm_file_name = "Sys.vm";

void process_file(const string &input_file_or_dir, const string &output_file, const translator::Config &config) {
    const string res = Translator(config).translate(input_file_or_dir);
    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        out_file << res;
        out_file.close();
    }
}


void link_files(vector<filesystem::path> input_files) {
    assert(!input_files.empty() && "Input files can't be empty when linking them");
    auto linked_file = input_files[0];

    linked_file.replace_filename(lined_file_name + assembler_extension);

    cout << "Linking files into " << linked_file << endl;

    std::filesystem::remove(linked_file);
    std::ofstream dst(linked_file, std::ios::binary);
    for (int i = 0; i < input_files.size(); ++i) {
        auto input_file = input_files[i];
        auto output_file = input_file;
        output_file.replace_extension(assembler_extension);

        if (i > 0) {
            dst << "\n\n\n";
        }
        //TODO: add check if unknown function is called
        dst << "// File " << input_file.filename().string() << "\n";
        std::ifstream src(output_file, std::ios::binary);
        dst << src.rdbuf();
        src.close();
    }
    dst.close();
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
    bool has_sys_vm = false;
    for (int i = 0; i < input_files.size(); ++i) {
        if (input_files[i].filename().string() == sys_vm_file_name) {
            has_sys_vm = true;
            std::swap(input_files[i], input_files.front());
            break;
        }
    }
    if (has_sys_vm) {
        cout << "Input dir has Sys.vm file" << endl;
    }
    for (auto &input_file: input_files) {
        auto output_file = input_file;
        output_file.replace_extension(assembler_extension);
        cout << "Processing " << input_file << endl << "into " << output_file << endl;
        if (has_sys_vm) {
            if (input_file.filename().string() == sys_vm_file_name) {
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
    link_files(input_files);
}

int main(int argc, char *argv[]) {
    CLI::App app{"Hack VM translator.\nTranslate a .vm file into .hack file"};
    argv = app.ensure_utf8(argv);
    string input_file_or_dir, memory_segments;
    bool debug = false, memory_init = false, clear_stack = false, disable_comments=false;
    app.add_option("input", input_file_or_dir, "input file directory path")
            ->check(CLI::ExistingPath)->required();
    app.add_flag("-d,--debug", debug, "Debug mode");
    app.add_flag("--cs,--clear-stack", clear_stack, "Clear stack after each operation. Default: false");
    app.add_flag("--im,--initilize-memory", memory_init, "Initialize memory segments. Default: false");
    app.add_flag("--dc,--disable-comments", disable_comments, "Disable comments in files. Default: false");

    app.add_option("-m,--memory-segments", memory_segments, StringUtils::stripMargin(R"(
    |Initialize memory segments usign json in format {"segment": [minAdress, maxAdress]} For example
   |--memory-segments '{"stack": [256, 2047], "local": [2048, 2099], "arg": [3000, 3999], "this": [4000, 4999], "that": [5000, 5999], "pointer": [3, 4], "temp": [5, 12], "static": [16, 255]}'
    )"));
    CLI11_PARSE(app, argc, argv);
    if (debug) {
        cpptrace::register_terminate_handler();
    }

    if (memory_init && memory_segments.empty()) {
        throw runtime_error("Memory segments can't be empty when --initilize-memory flag is set");
    }
    const auto config = Config{
        memory_init ? MemorySegmentsParser::parse(memory_segments) : memory::default_memory_segment_min_max_adress,
        clear_stack, memory_init, !disable_comments
    };

    //check if there are .vm files inside input directory
    const filesystem::path path(input_file_or_dir);
    error_code ec; // For using the non-throwing overloads of functions below.
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

const map<string, memory::MemorySegment> segmentsTypes = {
    {"stack", memory::Stack},
    {"local", memory::Local,}, {"arg", memory::Arg}, {"this", memory::This},
    {"that", memory::That}, {"pointer", memory::Pointer}, {"temp", memory::Temp}, {"static", memory::Static}
};
