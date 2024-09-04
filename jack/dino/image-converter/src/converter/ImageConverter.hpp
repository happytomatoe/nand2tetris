#pragma once
#include <cmath>
#include <iostream>
#include <ostream>
#include <ranges>
#include <regex>
#include <string>
#include <re2/re2.h>

using namespace std;

struct Image {
    int rows = -1;
    int cols = -1;
    vector<vector<uint16_t> > pixels;

    Image(int height, int width, const uint16_t *pixels) {
        auto c = round((float) width / 8 / 2);

        cols = c;
        rows = height;
        //convert 1d vector to 2d array
        auto jLen = width / 8;
        for (int i = 0; i < height; ++i) {
            vector<uint16_t> temp;
            temp.reserve(width);
            for (int j = 1; j < jLen; j += 2) {
                auto val = pixels[i * width / 8 + j - 1] << 8 | pixels[i * width / 8 + j];
                temp.push_back(~val);
            }
            if (jLen % 2 != 0) {
                auto last_val = pixels[i * width / 8 + jLen - 1];
                auto val = last_val << 8 | 0b11111111;
                temp.push_back(~val);
            }
            std::reverse(temp.begin(), temp.end());
            //transform temp to string
            auto str = temp |
                       views::transform([](auto &&val) {
                           std::bitset<16> b(val);
                           return b.to_string();
                       }) | views::join;

            //res to string
            string s2;
            for (auto c: str) {
                s2 += c;
            }
            auto len = s2.length();
            cout << len;
            reverse(s2.begin(), s2.end());
            auto temp2 = s2 |
                         views::chunk(16) |
                         views::transform([](auto &&chunk) {
                             string res;
                             for (auto c: chunk) {
                                 res += c;
                             }
                             std::bitset<16> b(res);
                             uint16_t number = static_cast<uint16_t>(b.to_ullong() & 0xFFFF);
                             return number;
                         })
                         //to vector
                         | ranges::to<vector<uint16_t> >();
            this->pixels.push_back(temp2);
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Image &obj) {
        os
                << "rows: " << obj.rows << endl
                << "cols: " << obj.cols << endl
                << "pixels: \n";
        for (int i = 0; i < obj.rows; ++i) {
            for (int j = 0; j < obj.cols; ++j) {
                std::bitset<16> b(obj.pixels[i][j]);
                os << b.to_string();
                os << " ";
            }
            os << endl;
        }

        return os;
    };
};

class ImageConverter {
public:
    static Image read(const string &text);

    static string convert(Image image);
};
