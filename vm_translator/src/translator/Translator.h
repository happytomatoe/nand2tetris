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
    string translate(const string &file_path,
                     const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAdress =
                             memory::defaultMemorySegmentMinMaxAdress);

    string translate(const vector<Token> &tokens, const string &file_name,
                     const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMax =
                             memory::defaultMemorySegmentMinMaxAdress);


    const static string program_end;

private:
    int stack_size = 0;
    int line_number = 0;
    int id = 0;
    const vector<memory::MemorySegment> memory_segments_to_save_on_function_call = {
        memory::MemorySegment::Local, memory::MemorySegment::Arg,
        memory::MemorySegment::This, memory::MemorySegment::That
    };

    static vector<memory::MemorySegment> reverse(const vector<memory::MemorySegment> &vector) {
        auto res = vector;
        std::reverse(res.begin(), res.end());
        return res;
    }

    vector<memory::MemorySegment> memory_segments_to_restore_on_function_return = reverse(
        memory_segments_to_save_on_function_call);

    static memory::Range get(const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress,
                             const memory::MemorySegment &p);

    static string initializeMemorySegments(const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAdress);

    static string getLine(const string &text, int line_number);

    string logicalComparison(TokenType type);

    string two_operand_operation(string operation);

    string handle_arithmetic_logical_operation(
        const int line_number, const Token &token);

    static string operationComment(TokenType operation, TokenType memorySementTokenType, int number);

    static string operationComment(TokenType operation);

    static string file_name_without_extension(const string &file_name);

    string handle_push(const string &file_name,
                       TokenType memorySementTokenType, int number,
                       const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress);

    string handle_pop(const string &file_name,
                      TokenType memorySementTokenType, int number,
                      const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMaxAddress);

    string stackPush();

    string stackPop();

    int intRand(const int &min, const int &max);

    void checkAdressOutOfRange(const int &value,
                               const map<memory::MemorySegment, memory::Range> &memorySegmentsMinMax,
                               const memory::MemorySegment &p) const;

    void check_overflow(int value) const;
};


#endif //TRANSLATOR_H
