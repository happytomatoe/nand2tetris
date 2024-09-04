#pragma once
#include <cmath>
#include <ostream>
#include <ranges>
#include <regex>
#include <string>
#include <utility>
#include <cpptrace/cpptrace.hpp>
#include <re2/re2.h>

using namespace std;

class ImageConverter {
public:
    static vector<string> read(const string &text);

    static string convert(const vector<vector<string> > &img, const bool comments = true);

    static vector<vector<string> > normalize(vector<string> input) {
        vector<vector<string> > res;
        res.reserve(input.size());
        auto t = input[0].length();
        const auto row_bit_count = ceil(double(t) / 16) * 16;
        if (row_bit_count <= 0)throw cpptrace::domain_error("Expected cols count to be > 0");
        for (auto temp: input) {
            if (temp.length() < row_bit_count) temp.append(row_bit_count - temp.length(), '1');
            //flip horizontally
            ranges::reverse(temp);
            vector<string> elem = temp
                                  | views::chunk(16)
                                  | views::transform([](auto &&chunk) {
                                      string res;
                                      for (auto c: chunk) {
                                          res += c;
                                      }
                                      std::bitset<16> b(res);
                                      //negate bits
                                      return (~b).to_string();
                                  }) | ranges::to<std::vector<std::string> >();
            ranges::reverse(elem);

            res.push_back(elem);
        }
        if (res.size() != input.size())
            throw cpptrace::domain_error("Something went wrong");
        return res;
    }


private:
    static int count_rows(const string &string);
};
