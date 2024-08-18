//
// Created by babkamen on 8/18/24.
//

#include "Parser.h"

#include <vector>
struct Token;


enum StatementKind {
    Assignment,
    Jump,
    At
};
// struct AST {
//     std::vector<Statement*> statements;
// };
//
//
// Node Parser::parse(std::vector<Token> tokens) {
//     Node* root = new Node(0);
//     return ;
// }
