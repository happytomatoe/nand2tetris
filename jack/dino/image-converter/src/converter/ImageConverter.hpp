#pragma once
#include <cmath>
#include <ostream>
#include <regex>
#include <string>
#include <re2/re2.h>

using namespace std;

struct Image {
    int rows = -1;
    int cols = -1;
    vector<vector<uint16_t> > pixels;

    Image(int height, int width, const uint16_t *pixels) {
        cols = round((float) width / 16);
        rows = height;
        //convert 1d vector to 2d array
        for (int i = 0; i < height; ++i) {
            vector<uint16_t> temp;
            temp.reserve(width);
            for (int j = 1; j < width / 8; j += 2) {
                auto val = pixels[i * width / 8 + j - 1] << 8 | pixels[i * width / 8 + j];
                temp.push_back(~val);
            }
            std::reverse(temp.begin(), temp.end());
            this->pixels.push_back(temp);
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
