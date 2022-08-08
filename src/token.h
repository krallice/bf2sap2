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
    size_t position;

    Token(BfTokenType t, int c, size_t p) {
      tokentype = t;
      count = c;
      position = p;
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