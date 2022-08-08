#ifndef TOKEN_H
#define TOKEN_H

enum class BfTokenType {
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
    BfTokenType tokentype;
    int count;
    size_t position; // Original position in source code
    size_t matching_loop_index; // Index of the matching loop command, set to -1 for non loop tokens

    Token(BfTokenType t, int c, size_t p) {
      this->tokentype = t;
      this->count = c;
      this->position = p;
      this->matching_loop_index = -1;
    }

    int is_compressible() const {
      if ( tokentype == BfTokenType::INCREMENT_PTR || tokentype == BfTokenType::DECREMENT_PTR ||
          tokentype == BfTokenType::INCREMENT_DATA || tokentype == BfTokenType::DECREMENT_DATA ) {
        return 1;
      } else {
        return 0;
      }
    }
};

#endif