
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
#ifdef NDEBUG
    printf("Release configuration!\n");
#else
    printf("Debug configuration!\n");
#endif
    if (argc < 2) {
        std::cerr << "missing input file!\n";
        std::cerr << "usage: assembler <input_file> ...\n";
        return EXIT_FAILURE;
    }
    std::cout << "Reading file: " << argv[1] << "" << std::endl;
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr<< "Failed to open file. Exiting out...";
        exit(0); //failed to open file
    }
    std::vector<std::string> lines;
    std::string str;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    file.close();

    std::string file_contents;
    for (std::string line : lines) {
        file_contents += line;
        file_contents += "\n";
    }
    // std::string assembledText = assemble(file_contents);
    // std::cout << assembledText;
    //close file
    return EXIT_SUCCESS;
}
