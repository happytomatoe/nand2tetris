#include "Assembler.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <vector>
#include <cpptrace/cpptrace.hpp>

#include "Lexer.h"
#include "Parser.h"
#include "StringUtils.h"


using namespace std;


const map<TokenType, int> destBitsMap = {
    {M, 0b1},
    {D, 0b10},
    {A, 0b100}
};
const map<list<TokenType>, int> compBitsMap = {
    {{Zero}, 0b101010},
    {{One}, 0b111111},
    {{NegativeOne}, 0b111010},
    {{D}, 0b001100},
    {{A}, 0b110000},
    {{M}, 0b110000},
    {{Not, D}, 0b001111},
    {{Not, A}, 0b110001},
    {{Not, M}, 0b110001},
    {{Minus, D}, 0b001111},
    {{Minus, A}, 0b110011},
    {{Minus, M}, 0b110011},
    {{D, Plus, One}, 0b011111},
    {{A, Plus, One}, 0b110111},
    {{M, Plus, One}, 0b110111},
    {{M, Plus, One}, 0b110111},
    {{D, Minus, One}, 0b001110},
    {{A, Minus, One}, 0b110010},
    {{M, Minus, One}, 0b110010},
    {{D, Plus, A}, 0b000010},
    {{D, Plus, M}, 0b000010},
    {{D, Minus, A}, 0b010011},
    {{D, Minus, M}, 0b010011},
    {{A, Minus, D}, 0b000111},
    {{M, Minus, D}, 0b000111},
    {{D, And, A}, 0b000000},
    {{D, And, M}, 0b000000},
    {{D, Or, A}, 0b010101},
    {{D, Or, M}, 0b010101},
};

string Assembler::assemble(const string &file_path) {
    string result;
    unsigned i = 0;
    std::ifstream file(file_path);
    if (!file.good()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }
    std::string line;
    auto lineTokens = make_shared<vector<vector<Token> > >();
    while (file.good() && std::getline(file, line)) {
        try {
            auto tokens = Lexer::lex(line);
            if (tokens[0].type == EOL || tokens[0].type == Eof) {
                i++;
                continue;
            }
            lineTokens->push_back(tokens);
            i++;
        } catch ([[maybe_unused]] exception &e) {
            cout << "Exception on line " + to_string(i) + ": " << line << endl;
            throw;
        }
    }
    //TODO: add exception if file is not good
    file.close();
    auto p = make_unique<Parser>();

    for (const auto &t: *lineTokens) {
        p->parse_only_labels(t);
    }
    p->reset();
    for (const auto &t: *lineTokens) {
        if (auto ast = p->parse(t); ast != nullptr) {
            result += assemble(ast) + "\n";
        }
    }


    return result;
}

string Assembler::assemble(const unique_ptr<TreeNode> &root) {
    if (root->token.category == AtCategory) {
        const auto value = root->right->token.constValue;
        return '0' + std::bitset<15>(value).to_string();
    }
    int jumpBits = 0b000;
    int destBits = 0b000;
    int compBits = 0b000000;
    int a = 0b0;
    switch (root->token.category) {
        case AssignmentOperation: {
            destBits = destBitsMap.at(root->left->token.type);
            if (root->left->left != nullptr) {
                destBits += destBitsMap.at(root->left->left->token.type);
            }
            if (root->left->right != nullptr) {
                destBits += destBitsMap.at(root->left->right->token.type);
            }
            if (root->right->token.category == PredefinedConstant) {
                compBits = compBitsMap.at({root->right->token.type});
            } else if (root->right->token.category == Identifier) {
                compBits = compBitsMap.at({root->right->token.type});
                if (root->right->token.type == M) a = 0b1;
            } else if (root->right->token.category == OtherOperation) {
                const auto operation = std::move(root->right);
                list<TokenType> operation_token_types = {
                    operation->token.type,
                    operation->right->token.type,
                };
                if (operation->left != nullptr) {
                    operation_token_types.push_front(operation->left->token.type);
                }
                compBits = compBitsMap.at(operation_token_types);
                if (operation->left != nullptr && operation->left->token.type == M || operation->right->token.type ==
                    M) {
                    a = 0b1;
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
                a = 0b1;
            }
            //operation and jump
            if (root->right->right != nullptr) {
                jumpBits = jumpTypeToBits(root->right->right->token.type);
            }
            break;
        }
        case Identifier:
        case PredefinedConstant: {
            //dest comp jump
            compBits = compBitsMap.at({root->token.type});
            if (root->right != nullptr && root->right->token.category == Jump) {
                jumpBits = jumpTypeToBits(root->right->token.type);
            }
            break;
        }
        default: {
            const string s = "Unexpected token: ";
            throw cpptrace::logic_error(s + StringUtils::to_string(root->token));
        }
    }
    constexpr auto instruction = 0b111;

    return std::bitset<16>(instruction << 13 | a << 12 | compBits << 6 | destBits << 3 | jumpBits).to_string();
}


int Assembler::jumpTypeToBits(TokenType type) {
    switch (type) {
        case JGT:
            return 0b001;
        case JEQ:
            return 0b010;
        case JGE:
            return 0b011;
        case JLT:
            return 0b100;
        case JNE:
            return 0b101;
        case JLE:
            return 0b110;
        case JMP:
            return 0b111;
        default:
            string s = "Unexpected jump type: ";
            throw cpptrace::logic_error(s + Token::toString(type));
    }
}
