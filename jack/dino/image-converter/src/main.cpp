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
    CLI::App app{"Convert black and white image to Jack code"};
    argv = app.ensure_utf8(argv);
    string input_file;
    optional<string> config_file;
    bool debug, copy_to_clipboard;
    app.add_option("input", input_file, "input file path")
            ->check(CLI::ExistingFile)->required();
    app.add_flag("-d,--debug", debug, "Debug mode");
    app.add_flag("-c,--clip", copy_to_clipboard, "Copy output to clipboard");

    CLI11_PARSE(app, argc, argv);
    if (debug) {
        cpptrace::register_terminate_handler();
    }

    const auto res = ImageConverter::convert(input_file);
    if (copy_to_clipboard) {
        clip::set_text(res);
        cout << "Copied to clipboard" << endl;
    } else {
        cout << res;
    }
    clip::set_text(res);
    return 0;
}
