#include "ImageConverter.hpp"
#include <iostream>
#include <ranges>
#include "StringUtils.cpp"

Image ImageConverter::read(const string &text) {
    int height, width;

    RE2 height_re("HEIGHT ([\\d]+)");
    RE2 width_re("WIDTH ([\\d]+)");
    assert(height_re.ok());
    assert(RE2::PartialMatch(text, height_re, &height) && "Cannot find height");
    assert(RE2::PartialMatch(text, width_re, &width) && "Cannot find width");

    string array_search_res;
    RE2 re("(?m){([\n\t0-9xa-fA-F,\\s]+)}");
    assert(re.ok());

    if (!RE2::PartialMatch(text, re, &array_search_res)) {
        throw runtime_error("Cannot find array with hexadecimal numbers");
    }
    vector<uint16_t> pixels = array_search_res
                              | views::split(',')
                              | views::transform([](auto &&str) {
                                  const auto s = string_view(&*str.begin(), ranges::distance(str));
                                  string n;
                                  if (!RE2::PartialMatch(s, "(0[xX][0-9a-fA-F]+)", &n)) {
                                      throw runtime_error(
                                          "Array should only contain numbers in hex but got '" + string(s) + "'");
                                  }
                                  std::stringstream ss;
                                  ss << std::hex << n;
                                  uint16_t numb;
                                  ss >> numb;
                                  return numb;
                              })
                              | ranges::to<vector<uint16_t> >();
    string filename;
    RE2 filename_re("([a-z_]+)\\[\\]");
    assert(re.ok());

    if (!RE2::PartialMatch(text, filename_re, &filename)) {
        throw runtime_error("Cannot find array with hexadecimal numbers");
    }
    return Image{height, width, pixels, filename};
}

string ImageConverter::convert(const Image &img, const bool comments) {
    /**
    * Mapping
    The (row, col) pixel in the physical screen is represented by
    the (col % 16)th bit in RAM address SCREEN + 32* row + col /16
    */
    std::string header = "function void draw" + StringUtils::snake_case_to_camel_case(img.file_name) + "(int location) {\n";
    if (comments)
        header += "   //img height: " + std::to_string(img.rows) + " width: " + std::to_string(img.cols) + "\n";
    header += "   var int memAddress;\n";
    header += "   let memAddress=16384 + location;\n";
    string res = header;
    for (int i = 0; i < img.rows; ++i) {
        if (comments)
            res += "|   //row: " + to_string(i) + "\n";
        for (int j = 0; j < img.cols; ++j) {
            std::bitset<16> b3(img.pixels[i][j]);
            auto number = static_cast<short>(b3.to_ullong() & 0xFFFF);
            if (number != 0) {
                if (comments) {
                    if (j > 0) {
                        res += format("|   //cols: {}-{} bits {}\n", to_string(j * 16), to_string((j + 1) * 16),
                                      b3.to_string());
                    } else {
                        res += format("|   //cols: 0-16 bits {}\n", b3.to_string());
                    }
                }
                int memAddress = 32 * i + j;
                string addr = memAddress == 0 ? "" : "+" + to_string(memAddress);
                res += format("|   do Memory.poke(memAddress{}, {});\n", addr, number);
            }
        }
    }
    string footer = R"(|   return;
         |})";
    return StringUtils::stripMargin(res + footer);
}


/**
 *The Hack computer employs a memory-mapped approach to I/O.
 *Bitmapped, black and white output to a virtual 256 x 512 screen is
 *effected by writing a bitmap of the desired output to data memory locations 16384 (0x4000) through 24575 (0x5FFF).
 *The data words in this address range are viewed as a linear array of bits
 *with each bit value representing the black/white state of a single pixel on the computer emulator's virtual screen.
 *The least significant bit of the word in the first memory address of the screen RAM segment sets the pixel in
 *the upper left corner of the screen to white if it is 0 and black if it is 1.
 *The next-most significant bit in the first word controls the next pixel to the right, and so on.
 *After the first 512-pixel row is described by the first 32 words of screen memory,
 *the mapping is continued in the same fashion for the second row with the next 32 words.
 *Logic external to the computer reads the screen RAM memory map segment and updates the virtual screen.
*/
