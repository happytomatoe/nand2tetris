#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <vector>
#include <regex>

#include "Lexer.h"
using namespace std;

struct Destination {
    bool a, d, m;

    string toBinary() {
        string res = "";
        if (a) {
            res += "1";
        } else {
            res += "0";
        }
        if (d) {
            res += "1";
        } else {
            res += "0";
        }
        if (m) {
            res += "1";
        } else {
            res += "0";
        }
        return res;
    }
};

optional<tuple<Destination, int>> get_destination(std::vector<Token> tokens) {
    Destination dest = {false, false, false};
    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == Assignment) {
            for (int j = i - 1; j >= 0; j--) {
                switch (tokens[j].type) {
                    case A:
                        dest.a = true;
                    break;
                    case D:
                        dest.d = true;
                    break;
                    case M:
                        dest.m = true;
                    break;
                    default:
                        throw std::invalid_argument(
                            "Cannot assign to " + tokens[j].text + ". Possible values=AMD");
                }
            }
            return std::optional<tuple<Destination, int>>{{dest, i}};
        }
    }
    return nullopt;
}


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
            auto maybeDestination = get_destination(tokens);
            if (maybeDestination.has_value()) {
                auto [destination, index] = maybeDestination.value();
                // s += destination.toBinary();
                // for (int j = index + 1; j < tokens.size(); ++j) {
                //     s += tokens[j].text;
                // }
            }
            //check for operation
            // for(int k = j; k < tokens.size(); ++k){
            //
            // }

            //comp bits
            //a=0 or a==1
            //jumps
        }
        res.push_back(s);
    }

    return res;
}
