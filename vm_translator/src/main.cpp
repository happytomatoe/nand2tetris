#include <cpptrace/cpptrace.hpp>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "StringUtils.h"
#include "translator/MemorySegmentsParser.h"
#include "translator/Translator.h"
using namespace std;

constexpr auto assembler_extension = ".asm";
constexpr string vm_file_extension = ".vm";
constexpr string lined_file_name = "Linked";

void process_file(const string &input_file_or_dir, const string &output_file,
                  const translator::Config &config) {
    const string res = Translator(config).translate(input_file_or_dir);
    if (output_file.empty()) {
        cout << res;
    } else {
        ofstream out_file(output_file);
        out_file << res;
        out_file.close();
    }
}

void link_files(vector<filesystem::path> output_files) {
    assert(!output_files.empty() &&
           "Output files can't be empty when linking them");
    auto linked_file = output_files[0];

    linked_file.replace_filename(lined_file_name + assembler_extension);

    cout << "Linking files into " << linked_file << endl;

    std::filesystem::remove(linked_file);
    std::ofstream dst(linked_file, std::ios::binary);

    for (int i = 0; i < output_files.size(); ++i) {
        const auto output_file = output_files[i];
        if (i > 0) {
            dst << "\n\n\n";
        }
        // TODO: add check if unknown function is called
        dst << "// File " << output_file.filename().string() << "\n";
        std::ifstream src(output_file, std::ios::binary);

        if (i == 0 && output_file.stem() == "Main") {
            // store src content into a string
            string s;
            std::stringstream buffer;
            buffer << src.rdbuf();
            s = buffer.str();
            auto main_func_pos = s.find("(Main.main)");
            // check if main is first function inside the file

            if (main_func_pos == string::npos) {
                throw cpptrace::domain_error(
                    "Main.main function not found in Main.asm file");
            }
            auto first_function = s.find("(Main.");
            if (main_func_pos != first_function) {
                auto next_func_pos = s.find("(Main.", main_func_pos + 1);
                string main_func = s.substr(main_func_pos, next_func_pos - main_func_pos);
                if (main_func.back() == '\n') {
                    main_func.pop_back();
                }
                s.erase(main_func_pos, next_func_pos - main_func_pos);
                auto insert_pos = first_function - 1;
                s.insert(insert_pos > 0 ? insert_pos : 0, "\n" + main_func);
                dst << s;
            }
        } else {
            dst << src.rdbuf();
        }
        src.close();
    }
    dst.close();
}

void handle_dir(const filesystem::path &input_dir, const Config &config) {
    vector<filesystem::path> input_files = {};

    for (const auto &entry : filesystem::directory_iterator(input_dir)) {
        if (is_regular_file(entry) &&
            entry.path().extension() == vm_file_extension) {
            input_files.push_back(entry.path());
        }
    }
    if (input_files.empty()) {
        throw std::runtime_error(format(
            "No {} files found in the input directory", vm_file_extension));
    }

    const auto main =
        std::find_if(input_files.begin(), input_files.end(),
                     [](const auto &file) { return file.stem() == "Main"; });
    const auto has_main = main != input_files.end();
    if (has_main) {
        std::swap(input_files[0], *main);
    }
    const auto sys_vm =
        std::find_if(input_files.begin(), input_files.end(),
                     [](const auto &file) { return file.stem() == "Sys"; });

    if (has_main) {
        cout << "Input dir has Main.vm file" << endl;
    }
    bool has_sys_vm = sys_vm != input_files.end();
    if (has_sys_vm) {
        std::swap(input_files[0], *sys_vm);
    }
    if (has_sys_vm) {
        cout << "Input dir has Sys.vm file" << endl;
    }
    vector<filesystem::path> output_files;
    for (int i = 0; i < input_files.size(); i++) {
        auto &input_file = input_files[i];
        auto output_file = input_file;
        output_file.replace_extension(assembler_extension);
        output_files.push_back(output_file);
        cout << "Processing " << input_file << endl;
        if (i == 0 && (has_sys_vm | has_main)) {
            process_file(input_file.string(), output_file, config);
        } else {
            if (has_sys_vm || has_main) {
                auto config_dup = config;
                config_dup.memory_init = false;
                process_file(input_file.string(), output_file, config_dup);
            } else {
                process_file(input_file.string(), output_file, config);
            }
        }
    }
    link_files(output_files);
}

int main(int argc, char *argv[]) {
    CLI::App app{"Hack VM translator.\nTranslate a .vm file into .hack file"};
    argv = app.ensure_utf8(argv);
    string input_dir, memory_segments;
    bool debug = false, disable_memory_init = false, clear_stack = false,
         disable_comments = false;
    app.add_option("input", input_dir, "input files directory path")
        ->check(CLI::ExistingDirectory)
        ->required();
    app.add_flag("-d,--debug", debug, "Debug mode");
    app.add_flag("--cs,--clear-stack", clear_stack,
                 "Clear stack after each operation. Default: false");
    app.add_flag("--dmi,--disable-memory-initiailization", disable_memory_init,
                 "Initialize memory segments. Default: false");
    app.add_flag("--dc,--disable-comments", disable_comments,
                 "Disable comments in files. Default: false");

    app.add_option("-m,--memory-segments", memory_segments,
                   StringUtils::stripMargin(R"(
    |Initialize memory segments usign json in format {"segment": [minAdress, maxAdress]} For example
   |--memory-segments '{"stack": [256, 2047], "local": [2048, 2099], "arg": [3000, 3999], "this": [4000, 4999], "that": [5000, 5999], "pointer": [3, 4], "temp": [5, 12], "static": [16, 255]}'
    )"));
    CLI11_PARSE(app, argc, argv);
    if (debug) {
        cpptrace::register_terminate_handler();
    }

    const auto config =
        Config{(!disable_memory_init & !memory_segments.empty())
                   ? MemorySegmentsParser::parse(memory_segments)
                   : memory::default_memory_segment_min_max_adress,
               clear_stack, !disable_memory_init, !disable_comments};

    // check if there are .vm files inside input directory
    const filesystem::path path(input_dir);
    error_code ec;  // For using the non-throwing overloads of functions below.
    if (is_directory(path, ec)) {
        handle_dir(path, config);
    }
    if (ec)  // Optional handling of possible errors.
    {
        cerr << "Error in is_directory: " << ec.message();
        return EXIT_FAILURE;
    }
    if (is_regular_file(path, ec)) {
        if (input_dir.find_last_of('.') == string::npos) {
            throw runtime_error("Input file has no extension");
        }
        const auto output_file =
            input_dir.substr(0, input_dir.find_last_of('.')) +
            assembler_extension;
        process_file(input_dir, output_file, config);
    }
    if (ec) {
        cerr << "Error in is_regular_file: " << ec.message();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

const map<string, memory::MemorySegment> segmentsTypes = {
    {"stack", memory::Stack},
    {
        "local",
        memory::Local,
    },
    {"arg", memory::Arg},
    {"this", memory::This},
    {"that", memory::That},
    {"pointer", memory::Pointer},
    {"temp", memory::Temp},
    {"static", memory::Static}};
