#pragma once
#include <chrono>
#include <climits>
#include <map>
#include <random>
#include <string>
#include <vector>
#include "Config.h"
#include "MemorySegment.h"
#include "Token.h"

using namespace std;
using namespace token;
using namespace translator;

class Translator {
public:
    const static Config default_config;
    Config config;
    Translator(Config config=default_config ):config(config){}
    string translate(const string &file_path);

    string translate(const vector<Token> &tokens, const string &file_name);

    const static string program_end;

private:
    static bool has_no_goto_or_if_goto(const vector<Token> &tokens);

    bool stack_size_counting_enabled = true;
    int stack_size = 0;
    int line_number = 0;
    const vector<memory::MemorySegment> memory_segments_to_save_on_function_call = {
        memory::MemorySegment::Local, memory::MemorySegment::Arg,
        memory::MemorySegment::This, memory::MemorySegment::That
    };

    vector<memory::MemorySegment> reverse(const vector<memory::MemorySegment> &vector) {
        auto res = vector;
        std::reverse(res.begin(), res.end());
        return res;
    }

    vector<memory::MemorySegment> memory_segments_to_restore_on_function_return = reverse(
        memory_segments_to_save_on_function_call);

    memory::Range get_memory_segment_min_max_address(const memory::MemorySegment &p) const;

    string initializeMemorySegments();

    [[nodiscard]] string handle_function_call(const Token &token);

    string handle_function_return();

    string handle_function_declaration(const Token &token);

    static string getLine(const string &text, int line_number);

    string logicalComparison(const TokenType &type);

    string two_operand_operation(const TokenType &operation, const string &operation_instructions);

    string handle_arithmetic_logical_operation(
        int line_number, const Token &token);

    string operationComment(TokenType operation, TokenType memorySementTokenType, int number);

    string operationComment(TokenType operation);

    string file_name_without_extension(const string &file_name);

    [[nodiscard]] string handle_push(const string &file_name,
                                     TokenType memorySementTokenType, int number);

    [[nodiscard]] string handle_pop(const string &file_name,
                                    TokenType memorySementTokenType, int number);

    string push_d_register_onto_stack();

    string stack_pop_into_d_register();

    static int random_int() {
        std::mt19937 generator(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());;
        std::uniform_int_distribution<int> distribution(0,INT_MAX);
        return distribution(generator);
    };

    void checkAdressOutOfRange(const int &value,
                               const memory::MemorySegment &p) const;

    void check_overflow(int number) const;
};
