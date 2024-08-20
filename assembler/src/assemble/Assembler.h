//
// Created by babkamen on 8/18/24.
//

#ifndef ASSEMBLE_H
#define ASSEMBLE_H
#include "Lexer.h"
#include "Parser.h"

class Assembler {
public:
    static string assemble(unique_ptr<TreeNode> root);
    static string assemble(string file_path);
private:
    static std::vector<std::string> read_file(const std::string& file_path);
    static string jumpTypeToBits(TokenType type);
};
#endif //ASSEMBLE_H
