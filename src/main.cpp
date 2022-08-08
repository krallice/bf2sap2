#include <iostream>
#include <vector>
#include <array>
#include <stack>

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
        tokens.emplace_back(BfTokenType::OUTPUT, 1, i);
        break;
      case ',':
        tokens.emplace_back(BfTokenType::INPUT, 1 ,i);
        break;
      case '>':
        tokens.emplace_back(BfTokenType::INCREMENT_PTR, 1 ,i);
        break;
      case '<':
        tokens.emplace_back(BfTokenType::DECREMENT_PTR, 1 ,i);
        break;
      case '+':
        tokens.emplace_back(BfTokenType::INCREMENT_DATA, 1, i);
        break;
      case '-':
        tokens.emplace_back(BfTokenType::DECREMENT_DATA, 1, i);;
        break;
      case '[':
        tokens.emplace_back(BfTokenType::START_LOOP, 1, i);
        break;
      case ']':
        tokens.emplace_back(BfTokenType::END_LOOP, 1, i);
        break;
      default:
        break;
    }
  }
  return;
}

void compress_tokenstring(const std::vector<Token>& lexerstring, std::vector<Token>& compressedstring) {

  // Iterate over the raw tokenstring
  std::vector<Token>::size_type i = 0;
  while (i < lexerstring.size()) {

    if (lexerstring[i].is_compressible()) {

      Token newtoken(lexerstring[i].tokentype, 1, i);

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

// Returns -1 on success:
int validate_tokenstring(std::vector<Token>& tokenstring, int& parser_error_char_number, std::string& parser_error_message) {

  // Stack to match our loop commands:
  std::stack<std::vector<Token>::size_type> loopstack;

  // Iterate over the raw tokenstring:
  std::vector<Token>::size_type i = 0;
  while (i < tokenstring.size()) {

    if (tokenstring[i].tokentype == BfTokenType::START_LOOP) {
        loopstack.push(i);
    } else if (tokenstring[i].tokentype == BfTokenType::END_LOOP) {
        if (loopstack.size() > 0) {
          // Link each loop command to each other:
          tokenstring[i].matching_loop_index = loopstack.top();
          tokenstring[loopstack.top()].matching_loop_index = i;
          loopstack.pop();
        } else {
          parser_error_char_number = tokenstring[i].position;
          parser_error_message = "Loop End without a matching Loop Start";
          return 1;
        }
    }
    i++;
  }

  if (loopstack.size() > 0) {
    parser_error_char_number = tokenstring[i].position;
    parser_error_message = "Loop Start without a matching Loop End";
    return 1;
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

  // Lexer:

  // First pass, convert our string to a vector of tokens:
  std::cout << "Phase 1a Lexer: Convert source bf string to vector of Tokens." << std::endl;
  std::vector<Token> lexerstring;
  lexer(program, lexerstring);

  // Second pass, compress multiple incremental instructions into single tokens:
  std::cout << "Phase 1b Lexer: Compress Token string." << std::endl;
  std::vector<Token> compressedstring;
  compress_tokenstring(lexerstring, compressedstring);

  // At this stage, compressedstring contains our tokenized version of the raw .bf program.
  // There is no promise that the program is gramatically valid, only that it contains all valid Tokens.

  // Parser:
  std::cout << "Phase 2a Parser: Syntax checking Loop commands." << std::endl;
  int parser_error_char_number = 0;
  std::string parser_error_message;
  int r = validate_tokenstring(compressedstring, parser_error_char_number, parser_error_message);
  if ( r != 0) {
    std::cout << "Phase 2a Parser: ERROR found in source code at <<Character " << parser_error_char_number << ">>" << std::endl;
    std::cout << "Phase 2a Parser: <<" << parser_error_message << ">>" << std::endl;
    return 1;
  }
  std::cout << "Phase 2a Parser: All looks OK." << std::endl;

  // Debug output:
  // std::cout << "TOKENTYPE COUNT POSITION" << std::endl;
  // for (auto c : compressedstring ) {
  //   std::cout << int(c.tokentype) << " " << int(c.count) << " " << int(c.position) << std::endl;
  // }

  compile_6502(compressedstring);

  return 0;
}