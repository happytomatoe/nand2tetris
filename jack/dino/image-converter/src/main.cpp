#include <iostream>
using namespace std;
#include "converter/ImageConverter.hpp"
#include <cpptrace/cpptrace.hpp>
#include "clip.h"

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

int main() {
    cpptrace::register_terminate_handler();
    string str;
    string text;

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
    auto image = ImageConverter::read(text).normalize();
    auto res = ImageConverter::convert(image, comments);
    if (yes_or_no_prompt("Copy to clipboard?(y/n)")) {
        clip::set_text(res);
    } else {
        cout << res;
    }
    clip::set_text(res);
    return 0;
}
