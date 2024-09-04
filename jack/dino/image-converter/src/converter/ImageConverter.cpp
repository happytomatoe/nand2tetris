#include "ImageConverter.hpp"
#include <ranges>
#include "StringUtils.h"

vector<string> ImageConverter::read(const string &text) {
    string array_search_res;
    vector<string> res;
    string row;
    for (auto c: text) {
        if (c == '\n' && !row.empty()) {
            res.push_back(row);
            row = "";
        } else if (c == '1' || c == '0') {
            row += c;
        }
    }
    return res;
}

string ImageConverter::convert(const vector<vector<string> > &img, const bool comments) {
    /**
    * Mapping
    The (row, col) pixel in the physical screen is represented by
    the (col % 16)th bit in RAM address SCREEN + 32* row + col /16
    */
    std::string header = "function void draw(int location) {\n";
    if (comments)
        header += "   //img height: " + std::to_string(img.size()) + " width: " + std::to_string(img[0].size()) + "\n";
    header += "   var int memAddress;\n";
    header += "   let memAddress=16384 + location;\n";
    string res = header;
    for (int i = 0; i < img.size(); ++i) {
        if (comments)
            res += "|   //row: " + to_string(i) + "\n";
        for (int j = 0; j < img[i].size(); ++j) {
            std::bitset<16> b3(img[i][j]);
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
                if (number < -32768 || number > 32767) {
                    throw cpptrace::domain_error("Unexpected number: " + to_string(number) + ". Bits " + b3.to_string());
                }
                res += format("|   do Memory.poke(memAddress{}, {});\n", addr, number);
            }
        }
    }
    string footer = R"(|   return;
         |})";
    return StringUtils::stripMargin(res + footer);
}

int ImageConverter::count_rows(const string &string) {
    std::string regexPunc = "(0x[\\da-zA-z]+,*\n)"; // matches any punctuations;
    re2::RE2 re2Punc(regexPunc);
    re2::StringPiece input(string);
    int numberOfMatches = 0;
    while (re2::RE2::FindAndConsume(&input, re2Punc)) {
        ++numberOfMatches;
    }
    return numberOfMatches;
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
