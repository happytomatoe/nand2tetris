#include "ImageConverter.hpp"

#include <fstream>
#include <lodepng.h>
#include "StringUtils.h"

string ImageConverter::convert(const string &filepath, const bool ignore_checksums, const bool debug) {
    std::vector<unsigned char> image;
    unsigned w;
    unsigned h;
    read(filepath, ignore_checksums, image, w, h);
    const auto img = convert_to_binary_image(image, w, h);
    if (debug) {
        std::cout << "Width: " << w << std::endl;
        std::cout << "Height: " << h << std::endl;
        std::cout << "Num pixels: " << w * h << std::endl;

        if (w > 0 && h > 0) {
            std::cout << "Top left pixel color:"
                    << " r: " << (int) image[0]
                    << " g: " << (int) image[1]
                    << " b: " << (int) image[2]
                    << " a: " << (int) image[3]
                    << std::endl;
        }
        cout << "Image lightness" << endl;
        cout << printImage(image, w, h) << endl;
        cout << "Binary image" << endl;
        StringUtils::print(img);
    }
    return convert_to_jack_code(img);
}

string ImageConverter::convert_to_jack_code(const vector<vector<bool> > &img) {
    string header = R"(
    |method void draw(int x, int y) {
    )";


    string res = header;
    for (int i = 0; i < img.size(); i++) {
        auto row = img[i];
        for (int j = 0; j < row.size(); j++) {
            if (!row[j]) {
                auto jStr = j > 0 ? format("+{}", j) : "";
                auto iStr = i > 0 ? format("+{}", i) : "";
                res += format("|   do Screen.drawVisiblePixel(x{},y{});\n", jStr, iStr);
            }
        }
    }
    auto footer = R"(
        |   return;
        |}
    )";
    return StringUtils::stripMargin(res + footer);
}


vector<vector<bool> > ImageConverter::convert_to_binary_image(const std::vector<unsigned char> &image, unsigned w,
                                                              unsigned h) {
    if (w <= 0 || h <= 0) {
        throw runtime_error("Invalid image size: " + to_string(w) + "x" + to_string(h));
    }
    vector res(h, vector(w, false));
    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w * 4; x += 4) {
            if (image[y * w * 4 + x] > 224) res[y][x / 4] = true;
        }
    }
    return res;
}

string ImageConverter::printImage(const std::vector<unsigned char> &image, unsigned w,
                                  unsigned h) {
    if (w <= 0 || h <= 0) {
        throw runtime_error("Invalid image size: " + to_string(w) + "x" + to_string(h));
    }
    string res;
    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w * 4; x += 4) {
            res += to_string(image[y * w * 4 + x]) + ",";
        }
        res += "\n";
    }
    return res;
}

void ImageConverter::read(const string &filepath, bool ignore_checksums, std::vector<unsigned char> &image, unsigned &w,
                          unsigned &h) {
    ifstream f(filepath.c_str());
    if (!f.good()) {
        throw runtime_error("File not found: " + filepath);
    }

    std::vector<unsigned char> buffer;

    lodepng::load_file(buffer, filepath); //load the image file with given filename

    lodepng::State state;
    if (ignore_checksums) {
        state.decoder.ignore_crc = 1;
        state.decoder.zlibsettings.ignore_adler32 = 1;
    }

    unsigned error = decode(image, w, h, state, buffer);

    if (error) {
        throw runtime_error("decoder error " + to_string(error) + ": " + lodepng_error_text(error));
    }
}
