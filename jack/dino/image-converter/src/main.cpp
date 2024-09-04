#include <iostream>
using namespace std;
#include "converter/ImageConverter.hpp"
#include <cpptrace/cpptrace.hpp>

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
    // auto image = ImageConverter::read(text);
    // cout << image;
    // cout << "pixels size: " << image.pixels.size() << endl;
    return 0;
}
