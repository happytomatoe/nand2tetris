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
    static string translate(const string &file_path,
                            const map<memory::MemorySegment, memory::Range>& memorySegmentsMinMaxAdress =
                                    memory::defaultMemorySegmentMinMaxAdress);

    static string translate(const vector<Token> &tokens, const string &file_name,
                            const map<memory::MemorySegment, memory::Range> &
                                    memorySegmentsMinMax = memory::defaultMemorySegmentMinMaxAdress);


    const static string program_end;

private:
    static memory::Range get(const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress,
                                               const memory::MemorySegment &p);

    static string initializeMemorySegments(const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAdress);

    static string getLine(const string &text, int line_number);

    static string logicalComparison(TokenType type);

    static string two_operand_operation(string operation);

    static string handle_arithmetic_logical_operation(
        int &stackSize, int line_number, Token token, const map<memory::MemorySegment, memory::Range> &
        memorySegmentsMinMaxAddress);

    static string operationComment(TokenType operation, TokenType memorySementTokenType, int number);

    static string operationComment(TokenType operation);

    static string file_name_without_extension(const string &file_name);

    static string handle_push(const string &file_name,
                              int &stackSize, const int line_number, TokenType memorySementTokenType, int number, const map<memory::MemorySegment,
                              memory::Range> &memorySegmentsMinMaxAddress);

    static string handle_pop(const string &file_name,
                             int &stackSize, const int line_number, TokenType memorySementTokenType, int number, const map<memory::MemorySegment,
                             memory::Range> &memorySegmentsMinMaxAddress);

    static string stackPush();

    static string stackPop();

    int intRand(const int &min, const int &max);

    static void checkAdressOutOfRange(const int &value, const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMax, const memory::MemorySegment &p, const
                                      int &line_number);

    static void check_overflow(int value, int line_number);
};


#endif //TRANSLATOR_H
