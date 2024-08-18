#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <regex>

#include "Lexer.h"
using namespace std;

struct Destination {
    bool a;
    bool d;
    bool m;
};

std::vector<std::string> assemble(std::vector<std::string> text) {
    vector<string> res = vector<string>();
    res.reserve(text.size());
    for (int i = 0; i < text.size(); ++i) {
        string s = "";
        auto tokens = Lexer::lex(text[i]);
        if (tokens[0].type == At) {
            s += "0";
            //check if digit
            if (tokens[1].type == Constant) {
                s += std::bitset<15>(stoi(tokens[1].text)).to_string();
            } else {
                cerr << "Invalid line" << text[i] << endl;
            }
        } else {
            s += "1";
            //does lexes have assignment
            if (find_if(tokens.begin(), tokens.end(),
                [](Token t) { return t.type == Assignment; }) != tokens.end()) {

            } else {

            }

            //destination
            //comp bits
            //a=0 or a==1
            //jumps
        }
        res.push_back(s);
    }

    return res;
}
