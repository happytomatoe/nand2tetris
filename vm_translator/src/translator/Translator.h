#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "MemorySegment.h"
#include "Token.h"

using namespace std;
using namespace token;

class Translator {
public:
    static string translate(const string &file_path);
    static string translate(const vector<Token> &tokens, const string &file_name);

private:
    static string operationComment(TokenType operation, TokenType memorySementTokenType, int number);
    static string handle_push(const string &file_name,
                              map<memory::MemorySegmentPointer, int> &pointerToAddress,
                              const int line_number, vector<Token>::const_iterator &it);



    static string handle_pop(map<memory::MemorySegmentPointer, int> &pointerToAddress, const int line_number, vector<Token>::const_iterator &it);
    static string stackPush();
    static string stackPop();
    static void check_overflow(int value);
};


#endif //TRANSLATOR_H
