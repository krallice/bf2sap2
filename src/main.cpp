#include <iostream>
#include <vector>
#include <array>

#include <fstream>
#include <sstream>

std::string ReadFileIntoString(const std::string& path) {
    auto ss = std::ostringstream{};
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int main(int argc, char** argv) {

  std::string program;
  try {
    program = ReadFileIntoString(argv[1]);
  } catch (...) {
    program = ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.";
  }
  
  return 0;
}