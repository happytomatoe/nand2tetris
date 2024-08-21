//
// Created by babkamen on 8/18/24.
//

#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <ostream>
#include <string>

#include "Lexer.h"
using namespace std;

struct TreeNode {
    Token token;
    unique_ptr<TreeNode> left, right;

    TreeNode(const Token data, TreeNode *parent = nullptr): token(data), left(nullptr), right(nullptr) {
    }

    friend bool operator==(const TreeNode &lhs, const TreeNode &rhs) {
        const auto t = lhs.token == rhs.token;
        const auto l = (lhs.left == nullptr && rhs.left == nullptr) ||
                       (lhs.left != nullptr && rhs.left != nullptr && *lhs.left == *rhs.left);
        const auto r = (!lhs.right && !rhs.right) ||
                       (lhs.right && rhs.right && *lhs.right == *rhs.right);
        return t && l && r;
    }


    friend bool operator!=(const TreeNode &lhs, const TreeNode &rhs) {
        return !(lhs == rhs);
    }

    friend ostream &operator <<(ostream &os, const TreeNode &obj) {
        os << endl;
        printBT("", os, obj, false);
        return os;
    }

private:
    friend void printBT(const std::string &prefix, ostream &os, const TreeNode &node, bool isLeft) {
        os << prefix;
        os << (isLeft ? "|--" : "L---");
        // print the value of the node
        os << node.token << std::endl;
        // enter the next tree level - left and right branch
        if (node.right != nullptr) {
            printBT(prefix + (isLeft ? "|\t" : "\t"), os, *node.right, true);
        }
        if (node.left != nullptr) {
            printBT(prefix + (isLeft ? "|   " : "    "), os, *node.left, false);
        }
    }
};


class Parser {
public:
    unique_ptr<TreeNode> parse();

    explicit Parser(const vector<Token> &tokens)
        : it(), tokens(&tokens) {
        it = tokens.begin();
    }

private:
    vector<Token>::const_iterator it;
    const vector<Token> *tokens;
    const map<string, unsigned short int> symbol_table = {
        {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
        {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
        {"SCREEN", 16384}, {"KBD", 24576}
    };
    unsigned short int free_address = 16;

    unique_ptr<TreeNode> operator_statement(vector<Token>::const_iterator &it,
                                            __detail::__unique_ptr_t<TreeNode> operationLeftIdentifier) const;

    unique_ptr<TreeNode> assigment_statement(unique_ptr<TreeNode> &assignmentIdentifier,
                                             vector<Token>::const_iterator &it);

    Token eat(vector<Token>::const_iterator &it, TokenType type) const;

    bool hasMoreTokens() {
        return it < tokens->end() && it->category != End;
    }
};


#endif //PARSER_H
