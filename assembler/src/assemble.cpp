
#include <string>
#include <vector>

std::vector<std::string> assemble(std::vector<std::string> text) {
     //trim white lines
     for (int i = 0; i < text.size(); ++i) {
          auto s = text[i];
          if(s.empty()) {
               text.erase(text.begin() + i);
               i--;
          }
     }
     //scan for sections

     return text;
}
