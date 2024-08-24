#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Token.h"

using namespace std;
using namespace token;

class Translator {
public:
    static string translate(const string &file_path);


    static string translate(const vector<Token> &tokens, const string &file_name);

private:
    static string stackPush();

    static void check_overflow(int value);
};


#endif //TRANSLATOR_H
