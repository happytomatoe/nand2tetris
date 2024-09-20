#include <filesystem>
#include <string>
#include <vector>

using namespace std;
class Linker {
   public:
    static void interlink(const vector<filesystem::path> &output_files,
                          const string &linked_file_name);
};