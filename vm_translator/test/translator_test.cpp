#include <list>
#include <regex>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "StringDiff.h"
#include "Utils.h"
#include "translator/MemorySegment.h"
#include "translator/Translator.h"
using namespace std;


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
    containsExpectedWithoutWhitespaces(Translator::translate(tokens, file_name), memoryInitAsembly);
}

// ********** PUSH ****************


TEST(TranslatorTest, PushConstant) {
    //push constant 1
    //TODO: use D=1
    vector<Token> tokens = {Token(Push), Token(ConstantMemorySegment), Token(Number, 1), Token(Eof)};
    for (auto token: tokens) {
        cout << token << endl;
    }
    auto actual = Translator::translate(tokens, file_name);
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
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
    containsExpectedWithoutWhitespaces(t, expected);
}

constexpr TokenType normalMemorySegments[] = {Local, Argument, This, That};

class NormalMemorySegmentTest : public testing::TestWithParam<pair<TokenType, int> > {
};

TEST_P(NormalMemorySegmentTest, Test) {
    //push mem segment 1
    vector<Token> tokens = {
        Token(Push), Token(GetParam().first), Token(Number, 2),
        Token(Eof)
    };
    for (auto token: tokens) {
        cout << token << endl;
    }
    auto actual = Translator::translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
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
        result.emplace_back(t, memory::getSymbolAdress(memory::tokenTypeToMemorySegmentPointer.at(t)));
    }

    return result;
}

INSTANTIATE_TEST_SUITE_P(TranslatorTest, NormalMemorySegmentTest, testing::ValuesIn(memorySegmentToSymbolAddress()));

TEST(TranslatorTest, PushTemp) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Temp), Token(Number, 2),
        Token(Eof)
    };
    for (auto token: tokens) {
        cout << token << endl;
    }
    auto actual = Translator::translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
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
    ASSERT_THROW(Translator::translate(tokens, file_name), InvalidOperation);
}


TEST(TranslatorTest, PushStatic) {
    //push static 1
    vector<Token> tokens = {
        Token(Push), Token(StaticMemorySegment), Token(Number, 2),
        Token(Eof)
    };
    for (auto token: tokens) {
        cout << token << endl;
    }
    auto actual = Translator::translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
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
        Token(Push), Token(Pointer), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator::translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
    // push pointer 0/1 *SP=THIS/THAT; SP++
    // pop pointer 0/1		SP--; THIS/THAT=*SP
    // 0=this address
    // 1=that address
    auto expected = R"(
        @3
        A=M
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
        Token(Push), Token(Temp), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator::translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
    // push pointer 0/1 *SP=THIS/THAT; SP++
    // pop pointer 0/1		SP--; THIS/THAT=*SP
    // 0=this address
    // 1=that address
    auto expected = R"(

    )";

    containsExpectedWithoutWhitespaces(t, expected);
}

TEST(TranslatorTest, PushPointerInvalid) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(Pointer), Token(Number, 2),
        Token(Eof)
    };
    EXPECT_THROW(Translator::translate(tokens, file_name), PointerOutOfRangeException);
}


// ****************** POP *************************
// pop mem_segment1 i      addr=LCL+i; SP--; *addr=*SP
TEST(TranslatorTest, PopThat) {
    //push constant 1
    vector<Token> tokens = {
        Token(Push), Token(ConstantMemorySegment), Token(Number, 1),
        Token(Pop), Token(That), Token(Number, 1),
        Token(Eof)
    };
    auto actual = Translator::translate(tokens, file_name);
    auto t = Utils::preprocess(actual);
    ASSERT_TRUE(Utils::replace(t,Utils::preprocess(memoryInitAsembly),""));
    // push pointer 0/1 *SP=THIS/THAT; SP++
    // pop pointer 0/1		SP--; THIS/THAT=*SP
    // 0=this address
    // 1=that address
    auto expected = R"(

    )";

    containsExpectedWithoutWhitespaces(t, expected);
}

//- pop static i
//pop temp i      addr=5+i; SP--; *addr=*SP
// pop pointer 0/1		SP--; THIS/THAT=*SP
// 0=this address
// 1=that address
