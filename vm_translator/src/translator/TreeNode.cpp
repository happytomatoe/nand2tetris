#include <memory>
#include "Token.h"
using namespace std;

struct TreeNode {
    Token token;
    unique_ptr<TreeNode> left, right;
    //TODO: Should the tree own the data?
    explicit TreeNode(const Token data, unique_ptr<TreeNode> &&left = nullptr,
                      unique_ptr<TreeNode> &&right = nullptr): token(data), left(move(left)),
                                                               right(move(right)) {
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
    friend void printBT(const string &prefix, ostream &os, const TreeNode &node, bool isLeft) {
        os << prefix;
        os << (isLeft ? "|--" : "L---");
        // print the value of the node
        os << node.token << endl;
        // enter the next tree level - left and right branch
        if (node.right != nullptr) {
            printBT(prefix + (isLeft ? "|\t" : "\t"), os, *node.right, true);
        }
        if (node.left != nullptr) {
            printBT(prefix + (isLeft ? "|   " : "    "), os, *node.left, false);
        }
    }
};
