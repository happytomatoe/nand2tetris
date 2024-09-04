#pragma once
#include <cmath>
#include <ostream>
#include <ranges>
#include <regex>
#include <string>
#include <utility>
#include <re2/re2.h>

using namespace std;

struct Image {
    int rows = -1;
    int cols = -1;
    string file_name;
    vector<vector<string> > pixels;

    Image(const int rows, const int cols, const vector<uint16_t> &pixels, const string &file_name): Image(
        rows, cols, pixels.data(), pixels.size()) {
        this->file_name = file_name;
    }

    Image(const int rows, const int cols, const vector<vector<string> > &pixels, string filename): rows(rows),
        cols(cols),
        pixels(pixels), file_name(std::move(filename)) {
    }

    Image(int rows, int width, const uint16_t *pixels, int array_size) {
        auto c = round((float) width / 8 / 2);

        cols = c;
        this->rows = rows;
        auto jLen = width / 8;

        for (int i = 0; i < rows; ++i) {
            string temp;
            temp.reserve(width);
            for (int j = 0; j < jLen; ++j) {
                auto ind = i * width / 8 + j;
                if (ind > array_size - 1) break;
                std::bitset<8> b(pixels[ind]);
                auto str = b.to_string();
                temp.append(str);
            }

            this->pixels.push_back({temp});
        }
    }

    Image normalize() {
        vector<vector<string> > res;
        res.reserve(rows);
        const auto row_bit_count = cols * 16;
        assert(row_bit_count>0);
        for (auto row: pixels) {
            assert(row.size()==1 && "Expected pixels row to have 1 element");
            auto temp = row[0];
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
            if (elem.size() != cols) {
                throw runtime_error(
                    "Expected cols count to be " + to_string(cols) + " but got " + to_string(elem.size()));
            }
            ranges::reverse(elem);

            res.push_back({elem});
        }
        if (res.size() != rows)
            throw runtime_error(
                "Expected rows count to be " + to_string(rows) + " but got " + to_string(res.size()));
        return Image{rows, cols, res, file_name};
    }

    friend std::ostream &operator<<(std::ostream &os, const Image &obj) {
        os
                << "rows: " << obj.rows << endl
                << "cols: " << obj.cols << endl
                << "pixels: \n";
        for (const auto &pixel: obj.pixels) {
            for (const auto &j: pixel) {
                os << j;
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

    static string convert(const Image &image, bool comments = true);
};
