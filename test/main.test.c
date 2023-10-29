#include "./lexer/lexer.test.h"

int main() {

  test_lexer_ident();
  test_lexer_free();
  test_lexer_int();
  test_lexer_keywords();
  test_lexer_string();
  test_lexer_operators();
  test_lexer_delimiters();

  return 0;
}
