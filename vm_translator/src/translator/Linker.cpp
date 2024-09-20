#include "Linker.h"

#include <assert.h>

#include <cpptrace/cpptrace.hpp>
#include <fstream>
#include <iostream>
using namespace std;

void Linker::interlink(const vector<filesystem::path> &output_files,
                              const string &linked_file_name) {
    assert(!output_files.empty() &&
           "Output files can't be empty when linking them");
    auto linked_file = output_files[0];

    linked_file.replace_filename(linked_file_name);

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
                string main_func =
                    s.substr(main_func_pos, next_func_pos - main_func_pos);
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
