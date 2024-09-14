#include <list>
#include <regex>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "StringDiff.h"
#include "Utils.h"
#include "translator/assembly.h"
#include "translator/MemorySegment.h"
#include "translator/StringUtils.h"
#include "translator/Translator.h"
using namespace std;

const string program_end = translator::stripMargin(R"(
        |(END)
        |    @END
        |    0;JMP
    )");
const map<memory::MemorySegment, memory::Range> memorySegmentMinMaxAdress = {
    {memory::Pointer, {3, 4}},
    {memory::Temp, {5, 12}},

    {memory::Static, {16, 255}},
    {memory::Stack, {256, 2047}},
    //Dynamically allocated
    {memory::Local, {2048, 2099}},
    {memory::Arg, {3000, 3999}},
    {memory::This, {4000, 4999}},
    {memory::That, {5000, 5999}},
    // we don't save const in memory. It is only used to get a value and push
    // it onto a stack
};
const auto config = Config{memorySegmentMinMaxAdress, false, false};
string memoryInitAsembly = R"(
    //memory init
    @256
    D = A
    @0
    M=D

    @2048
    D = A
    @1
    M=D

    @3000
    D = A
    @2
    M=D

    @4000
    D = A
    @3
    M=D

    @5000
    D = A
    @4
    M=D
    //memory init end
)";
string stackPop = translator::stripMargin(R"(
       |@0
       |M=M-1
       |A=M
       |D=M
)");
string file_name = "test.vm";

void containsExpectedWithoutWhitespaces(const string &actual, const string &expected) {
    auto trA = Utils::preprocess(actual);
    auto trE = Utils::preprocess(expected);
    cout << "Actual" << endl << trA << endl << "Expected:" << endl << trE << endl;
    auto ind = trA.find(trE);
    if (ind == std::string::npos) {
        auto diff = StringDiff::get_diff_multi_line_strings_in_reverse_order(trA, trE);
        FAIL() << "Diff: " << endl << diff;
    }
}

TEST(TranslatorTest, MemoryInit) {
    vector<Token> tokens = {Token(Eof)};
    containsExpectedWithoutWhitespaces(
        Translator({memorySegmentMinMaxAdress, false, true}).translate(tokens, file_name),
        memoryInitAsembly);
}

// ********** PUSH ****************


TEST(TranslatorTest, PushConstant) {
    //push constant 1
    //TODO: use D=1
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Eof)
    };
    for (const auto &token: tokens) {
        cout << token << endl;
    }
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    auto expected = R"(
        @1
        D=A
        @0
        A=M
        M=D
        @0
        M=M+1
    )";

    containsExpectedWithoutWhitespaces(t, expected);
}

constexpr TokenType normalMemorySegments[] = {Local, Argument, This, That};

class NormalMemorySegmentTest : public testing::TestWithParam<pair<TokenType, int> > {
};

TEST_P(NormalMemorySegmentTest, Push) {
    //push mem segment 1
    vector<Token> tokens = {
        Token(Push), Token(GetParam().first), Token(Number, 2),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    //addr=symbolAddress+i; *SP=*addr; SP++;
    auto expected = format(R"(
        @{}
        D=M
        @2
        D=D+A
        A=D
        D=M
        @0
        A=M
        M=D
        @0
        M=M+1

    )", GetParam().second);

    containsExpectedWithoutWhitespaces(t, expected);
}

list<pair<TokenType, int> > memorySegmentToSymbolAddress() {
    list<pair<TokenType, int> > result;
    for (auto t: normalMemorySegments) {
        result.emplace_back(t, memory::getSymbolAdress(memory::token_type_to_memory_segment.at(t)));
    }

    return result;
}

INSTANTIATE_TEST_SUITE_P(TranslatorTest, NormalMemorySegmentTest, testing::ValuesIn(memorySegmentToSymbolAddress()),
                         [](const testing::TestParamInfo<NormalMemorySegmentTest::ParamType>& info) {
                         return toString(info.param.first);
                         });

TEST(TranslatorTest, PushTemp) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Temp), Token(Number, 2),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    //addr=symbolAddress+i; *SP=*addr; SP++;
    auto expected = R"(
        @7
        D=M
        @0
        A=M
        M=D
        @0
        M=M+1
    )";

    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PushTempInvalid) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Temp), Token(Number, 8),
        Token(Eof)
    };
    for (auto token: tokens) {
        cout << token << endl;
    }
    ASSERT_THROW(Translator(config).translate(tokens, file_name), AdressOutOfMemorySegmentRange);
}


TEST(TranslatorTest, PushStatic) {
    //push static 1
    vector<Token> tokens = {
        Token(Push), Token(Static), Token(Number, 2),
        Token(Eof)
    };
    for (auto token: tokens) {
        cout << token << endl;
    }
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    /**
    @<file-name>.i
    stack.push(M)
    */
    auto expected = R"(
        @test.2
        D=M
        @0
        A=M
        M=D
        @0
        M=M+1
    )";

    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PushPointerThis) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Pointer), Token(Number, 0),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    // push pointer 0/1 *SP=THIS/THAT; SP++
    // pop pointer 0/1		SP--; THIS/THAT=*SP
    // 0=this address
    // 1=that address
    auto expected = R"(
        @3
        D=M
        @0
        A=M
        M=D
        @0
        M=M+1
    )";

    containsExpectedWithoutWhitespaces(t, expected);
}


TEST(TranslatorTest, PushPointerThat) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Pointer), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    // push pointer 0/1 *SP=THIS/THAT; SP++
    // pop pointer 0/1		SP--; THIS/THAT=*SP
    // 0=this address
    // 1=that address
    auto expected = R"(
        @4
        D=M
        @0
        A=M
        M=D
        @0
        M=M+1
    )";

    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PushPointerInvalid) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Pointer), Token(Number, 2),
        Token(Eof)
    };
    EXPECT_THROW(Translator(config).translate(tokens, file_name), PointerOutOfRangeException);
}


// ****************** POP *************************
// pop mem_segment1 i      addr=LCL+i; SP--; *addr=*SP
TEST_P(NormalMemorySegmentTest, Pop) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Pop), Token(GetParam().first), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = format(R"(
            @{}
            D=M
            @1
            D=D+A
            @pop_normal_segment_temp
            M=D
            @0
            M=M-1
            A=M
            D=M
            @pop_normal_segment_temp
            A=M
            M=D
    )", GetParam().second);
    containsExpectedWithoutWhitespaces(t, expected);
}


//- pop static i
//pop temp i      addr=5+i; SP--; *addr=*SP
// pop pointer 0/1		SP--; THIS/THAT=*SP
// 0=this address
// 1=that address
TEST(TranslatorTest, PopStatic) {
    auto number = 1;
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, number),
        Token(Pop), Token(Static), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    /**
    *- pop static i
    D=stack.pop
    @<file-name>.i
    M=D
    */
    auto expected = stackPop + format(R"(
       @test.{}
       M=D
    )", number);
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PopTemp) {
    auto number = 10;
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, number),
        Token(Pop), Token(Temp), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    /**
    pop temp i      addr=5+i; SP--; *addr=*SP
    */
    auto expected = stackPop + format(R"(
        @6
        M=D
    )", number);
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PopPointerThis) {
    auto number = 10;
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, number),
        Token(Pop), Token(Pointer), Token(Number, 0),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    cout << "Atual \n" << actual << endl;
    auto t = Utils::preprocess(actual);

    /**
    pop pointer 0/1		SP--; THIS/THAT=*SP
    0=this address
    1=that address    */
    auto expected = stackPop + format(R"(
        @3
        M=D
    )", number);
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PopPointerThat) {
    auto number = 10;
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, number),
        Token(Pop), Token(Pointer), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    /**
    pop pointer 0/1		SP--; THIS/THAT=*SP
    0=this address
    1=that address    */
    auto expected = stackPop + format(R"(
        @4
        M=D
    )", number);
    containsExpectedWithoutWhitespaces(t, expected);
}

// *********  Operations **************
const string stackPop2 = R"(
       @0
       M=M-1
       A=M
       D=M
       A=A-1
)";
TEST(TranslatorTest, Add) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Add),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = stackPop2 + R"(
       M=D+M
    )";
    containsExpectedWithoutWhitespaces(t, expected);
}


TEST(TranslatorTest, AddInvalid) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Add),
        Token(Eof)
    };
    ASSERT_THROW(Translator(config).translate(tokens, file_name), InvalidOperation);
}

TEST(TranslatorTest, Subtract) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Subtract),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = stackPop2 + R"(
       D=D-M
       M=-D
    )";
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, And) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 1),
        Token(And),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = stackPop2 + R"(
       M=D&M
    )";
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, Or) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Or),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = stackPop2 + R"(
       M=D|M
    )";
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, Negate) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Negate),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = R"(
       @0
       A=M-1
       M=-M
    )";
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, NegateInvalid) {
    vector<Token> tokens = {
        Token(Negate),
        Token(Eof)
    };
    ASSERT_THROW(Translator(config).translate(tokens, file_name), InvalidOperation);
}

string logicalComparison(assembly::Jump jump);

string remove_branch_rand_number(string t) {
    for (string s: {"IF_TRUE", "END_CHECK", "IF_FALSE"}) {
        regex reg(format("({})_[\\d]+", s));
        t = regex_replace(t, reg, "$1");
    }
    return t;
}

TEST(TranslatorTest, Equals) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Equals),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    t = remove_branch_rand_number(t);
    auto expected = logicalComparison(assembly::JEQ);
    containsExpectedWithoutWhitespaces(t, expected);
}

string logicalComparison(assembly::Jump jump) {
    string operation = jump == assembly::JEQ ? "D" : "-D";
    return format(R"(
       // pop last 2
       @0
       M=M-1
       A=M
       // get x
       D=M
       // get y
       A=A-1
       D=D-M
       @IF_TRUE
       {};{}
       // right result into x
       (IF_FALSE)
            @0
            A=M-1
            M=0
            @END_CHECK
            0;JMP
       (IF_TRUE)
            @0
            A=M-1
            M=1
       (END_CHECK)
    )", operation, to_string(jump));
}

TEST(TranslatorTest, GreaterThan) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 1),
        Token(GreaterThan),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    t = remove_branch_rand_number(t);
    auto expected = logicalComparison(assembly::Jump::JGT);
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, LessThan) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 1),
        Token(Push), Token(Constant), Token(Number, 11),
        Token(LessThan),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    t = remove_branch_rand_number(t);
    auto expected = logicalComparison(assembly::Jump::JLT);
    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, Not) {
    vector<Token> tokens = {
        Token(Push), Token(Constant), Token(Number, 0),
        Token(Not),
        Token(Eof)
    };
    auto actual = Translator(config).translate(tokens, file_name);
    auto t = Utils::preprocess(actual);

    auto expected = R"(
        @0
        A=M-1
        M=!M
    )";
    containsExpectedWithoutWhitespaces(t, expected);
}


void sigsegv_handler(int sig) {
    cpptrace::generate_trace().print();
    exit(1);
}


int main(int argc, char **argv) {
    signal(SIGSEGV, sigsegv_handler);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
