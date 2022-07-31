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

class Token {
  
  public:
    BFTOKEN tokentype;
    int count;

    Token(BFTOKEN t, int c) {
      tokentype = t;
      count = c;
    }

    int is_compressible() const {
      if ( tokentype == BFTOKEN::INCREMENT_PTR || tokentype == BFTOKEN::DECREMENT_PTR ||
          tokentype == BFTOKEN::INCREMENT_DATA || tokentype == BFTOKEN::DECREMENT_DATA ) {
        return 1;
      } else {
        return 0;
      }
    }
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

void lexer(const std::string& input, std::vector<Token>& tokens) {

  for(std::string::size_type i = 0; i < input.size(); i++) {

    switch(input[i]) {
      case '.':
        tokens.emplace_back(BFTOKEN::OUTPUT, 1);
        break;
      case ',':
        tokens.emplace_back(BFTOKEN::INPUT, 1);
        break;
      case '>':
        tokens.emplace_back(BFTOKEN::INCREMENT_PTR, 1);
        break;
      case '<':
        tokens.emplace_back(BFTOKEN::DECREMENT_PTR, 1);
        break;
      case '+':
        tokens.emplace_back(BFTOKEN::INCREMENT_DATA, 1);
        break;
      case '-':
        tokens.emplace_back(BFTOKEN::DECREMENT_DATA, 1);
        break;
      case '[':
        tokens.emplace_back(BFTOKEN::START_LOOP, 1);
        break;
      case ']':
        tokens.emplace_back(BFTOKEN::END_LOOP, 1);
        break;
      default:
        break;
    }
  }
  return;
}

void compressor(const std::vector<Token>& lexerstring, std::vector<Token>& compressedstring) {

  // Iterate over the raw tokenstring
  std::vector<Token>::size_type i = 0;
  while (i < lexerstring.size()) {

    if (lexerstring[i].is_compressible()) {

      Token newtoken(lexerstring[i].tokentype, 1);

      std::vector<Token>::size_type j = i + 1;
      while (j < lexerstring.size()) {
        if (lexerstring[j].tokentype == newtoken.tokentype) {
          newtoken.count++;
          j++;
        } else {
          break;
        }
      }

      // Push the compressed token onto the vector, jump our i forward:
      compressedstring.push_back(newtoken);
      i = j;

    // Otherwise just pop it straight onto the output vector:
    } else { 
      compressedstring.push_back(lexerstring[i]);
      i++;
    }
  }
}

int main(int argc, char** argv) {

  std::string program;
  try {
    program = ReadFileIntoString(argv[1]);
  } catch (...) {
    program = ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.";
  }

  // First pass, convert our string to a vector of tokens:

  std::cout << "Pass 1: Converting string to vector of Tokens." << std::endl;
  std::vector<Token> lexerstring;
  lexer(program, lexerstring);

  // Second pass, compress multiple incremental instructs into 
  std::cout << "Pass 2: Compressing Token string." << std::endl;
  std::vector<Token> compressedstring;
  compressor(lexerstring, compressedstring);

  for (auto c : compressedstring ) {
    std::cout << int(c.tokentype) << " " << int(c.count) << std::endl;
  }

  return 0;
}