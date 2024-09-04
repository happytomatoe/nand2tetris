#include <iostream>

#include "converter/StringUtils.h"
using namespace std;
#include "converter/ImageConverter.hpp"
#include <cpptrace/cpptrace.hpp>
#include "clip.h"
#include <CLI/CLI.hpp>

bool yes_or_no_prompt(const string &prompt) {
    cout << prompt << endl;
    string answer;
    while (true) {
        getline(cin, answer);
        if (answer == "y") {
            return true;
        } else if (answer == "n") {
            return false;
        } else {
            cout << "Please enter y or n" << endl;
        }
    }
}


int main(int argc, char *argv[]) {
    string str;
    string text;
    CLI::App app{"Hack vm translator"};
    argv = app.ensure_utf8(argv);
    string input_file_or_dir;
    optional<string> config_file;
    bool debug;
    app.add_flag("-d,--debug", debug, "Debug mode");
    CLI11_PARSE(app, argc, argv);
    if (debug) {
        cpptrace::register_terminate_handler();
    }

    // Prompt user to enter multiple lines of text
    cout << "Enter multiple lines of text and press enter couple of times: " << endl;
    bool first_empty_line = false;
    // Read input lines until an empty line is encountered
    while (getline(cin, str)) {
        if (str.empty()) {
            if (first_empty_line) {
                break;
            }
            first_empty_line = true;
        } else {
            first_empty_line = false;
        }

        text += str + "\n";
    }
    bool comments = yes_or_no_prompt("Do you want me to add comments?(y/n)");
    auto image = ImageConverter::read(text);
    if (debug) {
        StringUtils::print(image);
    }
    auto normalized = ImageConverter::normalize(image);
    if (debug) {
        cout << "Normalized" << endl;
        StringUtils::print(normalized);
    }
    auto res = ImageConverter::convert(normalized, comments);
    if (yes_or_no_prompt("Copy to clipboard?(y/n)")) {
        clip::set_text(res);
    } else {
        cout << res;
    }
    clip::set_text(res);
    return 0;
}
