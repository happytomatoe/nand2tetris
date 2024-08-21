//
// Created by babkamen on 8/18/24.
//

#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include "Lexer.h"

struct TreeNode;
using namespace std;

class Parser {
public:
    unique_ptr<TreeNode> parse(const vector<Token> &tokens);

    static map<string, int> predefined_symbols;

private:
    vector<Token>::const_iterator it;
    vector<Token> tokens;
    map<string, int> symbol_table = predefined_symbols;
    int free_address = 16;

    unique_ptr<TreeNode> operator_statement(vector<Token>::const_iterator &it,
                                            __detail::__unique_ptr_t<TreeNode> operationLeftIdentifier) const;

    unique_ptr<TreeNode> assigment_statement(unique_ptr<TreeNode> &assignmentIdentifier,
                                             vector<Token>::const_iterator &it);

    Token eat(vector<Token>::const_iterator &it, TokenType type) const;

    bool hasMoreTokens() {
        return it != tokens.end() && it->category != End;
    }
};


struct TreeNode {
    Token token;
    unique_ptr<TreeNode> left, right;
    //TODO: Should the tree own the data?
    explicit TreeNode(Token data): token(std::move(data)), left(nullptr), right(nullptr) {}

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

#endif //PARSER_H
