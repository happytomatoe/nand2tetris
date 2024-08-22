//
// Created by babkamen on 8/18/24.
//

#ifndef ASSEMBLE_H
#define ASSEMBLE_H
#include "Lexer.h"
#include "Parser.h"

class Assembler {
public:
    static string assemble(const unique_ptr<TreeNode> &root);
    static string assemble(const string &file_path);
private:
    static int jumpTypeToBits(TokenType type);
};
#endif //ASSEMBLE_H
