#include <algorithm>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <vector>
#include <regex>
#include <cpptrace/cpptrace.hpp>

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

optional<tuple<Destination, int> > get_destination(std::vector<Token> tokens) {
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
                        string s = "Cannot assign to " + tokens[j].type;
                        throw cpptrace::logic_error(s + +". Possible values=AMD");
                }
            }
            return std::optional<tuple<Destination, int> >{{dest, i}};
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
            if (tokens[1].type == Number) {
                s += std::bitset<15>(tokens[1].constValue).to_string();
            } else {
                cerr << "Invalid line" << text[i] << endl;
            }
        } else {
            s += "1";
            //does lexes have assignment
            auto maybeDestination = get_destination(tokens);
            if (maybeDestination.has_value()) {
                auto [destination, index] = maybeDestination.value();
                //check for operation
                for (int k = index; k < tokens.size(); ++k) {
                    switch (tokens[k].type) {
                        case Plus:
                            break;
                        case Minus:
                            break;
                        case And:
                            break;
                        case Or:
                            break;
                        case Not:
                            break;
                        default:
                            break;
                    }
                }
            } else {
            }

            //comp bits
            //a=0 or a==1
            //jumps
        }
        res.push_back(s);
    }

    return res;
}
