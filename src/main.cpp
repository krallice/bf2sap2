#include <iostream>
#include <vector>
#include <array>

#include <fstream>
#include <sstream>

enum class BFTOKEN {
  INCREMENT_PTR,
  DECREMENT_PTR,

  INCREMENT_DATA,
  DECREMENT_DATA,

  OUTPUT,
  INPUT,
  
  START_LOOP,
  END_LOOP,
};

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

int lexer(const std::string& input, std::vector<BFTOKEN>& tokens) {

  for(std::string::size_type i = 0; i < input.size(); i++) {

    switch(input[i]) {
      case '.':
        tokens.push_back(BFTOKEN::OUTPUT);
        break;
      case ',':
        tokens.push_back(BFTOKEN::INPUT);
        break;
      case '>':
        tokens.push_back(BFTOKEN::INCREMENT_PTR);
        break;
      case '<':
        tokens.push_back(BFTOKEN::DECREMENT_PTR);
        break;
      case '+':
        tokens.push_back(BFTOKEN::INCREMENT_DATA);
        break;
      case '-':
        tokens.push_back(BFTOKEN::DECREMENT_DATA);
        break;
      case '[':
        tokens.push_back(BFTOKEN::START_LOOP);
        break;
      case ']':
        tokens.push_back(BFTOKEN::END_LOOP);
        break;
      default:
        break;
    }
  }
  return 0;
}

int main(int argc, char** argv) {

  std::string program;
  try {
    program = ReadFileIntoString(argv[1]);
  } catch (...) {
    program = ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.";
  }

  std::vector<BFTOKEN> tokens;
  int lexerret = lexer(program, tokens);

  return 0;
}