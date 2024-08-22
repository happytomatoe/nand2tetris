#include "Translator.h"

#include <fstream>
#include <iostream>
#include <memory>

#include "Lexer.h"
#include "Parser.h"
#include "Token.h"

string Translator::translate(const string &file_path) {
    unsigned i = 0;
    ifstream file(file_path);
    if (!file.good()) {
        throw runtime_error("Failed to open file: " + file_path);
    }
    file.close();
    string line, text;

    while (file.good() && getline(file, line)) {
        text += line + "\n";
    }
    auto tokens = Lexer::lex(text);
    Parser::checkOrder(tokens);
    return translate(tokens);
}
