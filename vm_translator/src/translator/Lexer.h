#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Lexer {
public:
    static vector<Token> lex(const string &inputFile);
};



#endif //LEXER_H
