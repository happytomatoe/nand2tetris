#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <string>
#include <vector>

#include "Token.h"

using namespace std;

class Translator {
public:
    static string translate(const string &file_path);

private:
    static string translate(const vector<Token> &tokens);
};


#endif //TRANSLATOR_H
