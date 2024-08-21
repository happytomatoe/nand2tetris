#include "Assembler.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <regex>
#include <string>
#include <vector>
#include <regex>
#include <cpptrace/cpptrace.hpp>

#include "Lexer.h"
#include "Parser.h"
#include "StringUtils.h"
#include "../test/Utils.h"
using namespace std;


const map<TokenType, short> destBitsMap = {
    {M, 1},
    {D, 2},
    {A, 4}
};
const map<list<TokenType>, string> compBitsMap = {
    {{Zero}, "101010"},
    {{One}, "111111"},
    {{NegativeOne}, "111010"},
    {{D}, "001100"},
    {{A}, "110000"},
    {{M}, "110000"},
    {{Not, D}, "001111"},
    {{Not, A}, "110001"},
    {{Not, M}, "110001"},
    {{Minus, D}, "001111"},
    {{Minus, A}, "110011"},
    {{Minus, M}, "110011"},
    {{D, Plus, One}, "011111"},
    {{A, Plus, One}, "110111"},
    {{M, Plus, One}, "110111"},
    {{M, Plus, One}, "110111"},
    {{D, Minus, One}, "001110"},
    {{A, Minus, One}, "110010"},
    {{M, Minus, One}, "110010"},
    {{D, Plus, A}, "000010"},
    {{D, Plus, M}, "000010"},
    {{D, Minus, A}, "010011"},
    {{D, Minus, M}, "010011"},
    {{A, Minus, D}, "000111"},
    {{M, Minus, D}, "000111"},
    {{D, And, A}, "000000"},
    {{D, And, M}, "000000"},
    {{D, Or, A}, "010101"},
    {{D, Or, M}, "010101"},
};

string Assembler::assemble(const string &file_path) {
    string result;
    unsigned i = 0;
    for (const auto &line: read_file(file_path)) {
        try {
            auto tokens = Lexer::lex(line);
            if (tokens[0].type == EOL || tokens[0].type == Eof) {
                i++;
                continue;
            }
            auto p = make_unique<Parser>();
            auto ast = p->parse(tokens);
            result += assemble(ast) + "\n";
            i++;
        } catch ([[maybe_unused]] exception &e) {
            cout << "Exception on line " + to_string(i) + ": " << line << endl;
            throw;
        }
    }
    return result;
}

std::vector<std::string> Assembler::read_file(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file.good()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }
    std::vector<std::string> lines;
    std::string str;
    while (file.good() && std::getline(file, str)) {
        lines.push_back(str);
    }
    file.close();
    return lines;
}

string Assembler::assemble(const unique_ptr<TreeNode> &root) {
    if (root->token.category == AtCategory) {
        auto value = root->right->token.constValue;
        return "0" + std::bitset<15>(value).to_string();
    }
    const auto instruction = "111";
    string jumpBits = "000";
    string destBits = "000";
    string compBits = "000000";
    string a = "0";

    switch (root->token.category) {
        case AssignmentOperation: {
            const auto dest = destBitsMap.at(root->left->token.type);
            destBits = bitset<3>(dest).to_string();
            if (root->right->token.category == PredefinedConstant) {
                compBits = compBitsMap.at({root->right->token.type});
            } else if (root->right->token.category == Identifier) {
                compBits = compBitsMap.at({root->right->token.type});
                if (root->right->token.type == M) a = "1";
            } else if (root->right->token.category == OtherOperation) {
                auto operation = std::move(root->right);

                compBits = compBitsMap.at({
                    operation->left->token.type,
                    operation->token.type,
                    operation->right->token.type,
                });
                if (operation->left->token.type == M || operation->right->token.type == M) {
                    a = "1";
                }
                //operation and jump
                if (operation->right->right != nullptr) {
                    jumpBits = jumpTypeToBits(operation->right->right->token.type);
                }
            }
            break;
        }
        case OtherOperation: {
            //operation
            compBits = compBitsMap.at({
                root->left->token.type,
                root->token.type,
                root->right->token.type,
            });
            if (root->left->token.type == M || root->right->token.type == M) {
                a = "1";
            }
            //operation and jump
            if (root->right->right != nullptr) {
                jumpBits = jumpTypeToBits(root->right->right->token.type);
            }
            break;
        }
        case PredefinedConstant: {
            //dest comp jump
            compBits = compBitsMap.at({root->token.type});
            if (root->right != nullptr && root->right->token.category == Jump) {
                jumpBits = jumpTypeToBits(root->right->token.type);
            }
        }
        break;
        default: {
            string s = "Unexpected token: ";

            throw cpptrace::logic_error(s +
                                        StringUtils::to_string(root->token));
        }
    }
    return instruction + a + compBits + destBits + jumpBits;
}


string Assembler::jumpTypeToBits(TokenType type) {
    switch (type) {
        case JGT:
            return "001";
        case JEQ:
            return "010";
        case JGE:
            return "011";
        case JLT:
            return "100";
        case JNE:
            return "101";
        case JLE:
            return "110";
        case JMP:
            return "111";
        default:
            string s = "Unexpected jump type: ";
            throw cpptrace::logic_error(s + Token::toString(type));
    }
}
