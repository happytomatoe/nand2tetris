#pragma once
#include <regex>
#include <string>
#include <vector>

using namespace std;

class ImageConverter {
public:
    static string convert(const string &filepath, bool ignore_checksums = false, bool debug = false);

private:
    static string convert_to_jack_code(const vector<vector<bool> > &img);

    static vector<vector<bool> >
    convert_to_binary_image(const std::vector<unsigned char> &image, unsigned w, unsigned h);

    static string printImage(const std::vector<unsigned char> &image, unsigned w, unsigned h);

    static void read(const string &filepath, bool ignore_checksums, std::vector<unsigned char> &image, unsigned &w,
                     unsigned &h);
};
