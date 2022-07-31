#include <iostream>
#include <vector>
#include <array>

#include <fstream>
#include <sstream>

#include "token.h"
#include "6502.h"

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
        tokens.emplace_back(BfTokenType::OUTPUT, 1);
        break;
      case ',':
        tokens.emplace_back(BfTokenType::INPUT, 1);
        break;
      case '>':
        tokens.emplace_back(BfTokenType::INCREMENT_PTR, 1);
        break;
      case '<':
        tokens.emplace_back(BfTokenType::DECREMENT_PTR, 1);
        break;
      case '+':
        tokens.emplace_back(BfTokenType::INCREMENT_DATA, 1);
        break;
      case '-':
        tokens.emplace_back(BfTokenType::DECREMENT_DATA, 1);
        break;
      case '[':
        tokens.emplace_back(BfTokenType::START_LOOP, 1);
        break;
      case ']':
        tokens.emplace_back(BfTokenType::END_LOOP, 1);
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

  // Second pass, compress multiple incremental instructions into single tokens:
  std::cout << "Pass 2: Compressing Token string." << std::endl;
  std::vector<Token> compressedstring;
  compressor(lexerstring, compressedstring);

  // Debug output:
  for (auto c : compressedstring ) {
    std::cout << int(c.tokentype) << " " << int(c.count) << std::endl;
  }

  compile_6502(compressedstring);

  return 0;
}