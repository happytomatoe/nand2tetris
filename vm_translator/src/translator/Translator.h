#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "assembly.h"
#include "exception.h"
#include "MemorySegment.h"
#include "StringUtils.h"
#include "Token.h"

using namespace std;
using namespace token;

class Translator {
public:
    static string translate(const string &file_path);

    static string initializeMemorySegments();

    static string translate(const vector<Token> &tokens, const string &file_name);

    const static string program_end;

private:
    static string getLine(const string &text, int line_number);

    static string logicalComparison(TokenType type);

    static string two_operand_operation(char operation);

    static string handle_arithmetic_logical_operation(
        int &stackSize, int line_number, vector<Token>::const_iterator it);

    static string operationComment(TokenType operation, TokenType memorySementTokenType, int number);

    static string operationComment(TokenType operation);

    static string file_name_without_extension(const string &file_name);

    static string handle_push(const string &file_name,
                              int &stackSize, const int line_number, vector<Token>::const_iterator &it);

    static map<memory::MemorySegment, int> initMemorySegments();


    static string handle_pop(const string &file_name,
                             int &stackSize, const int line_number, vector<Token>::const_iterator &it);

    static string stackPush();

    static string stackPop();

    static void checkAdressOutOfRange(const int &value, const memory::MemorySegment &p, const int &line_number);

    static void check_overflow(int value, int line_number);
};


#endif //TRANSLATOR_H
