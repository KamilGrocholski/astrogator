#include "lexer/lexer.test.h"
#include "parser/parser.test.h"

int main() {

  test_lexer_ident();
  test_lexer_free();
  test_lexer_int();
  test_lexer_keywords();
  test_lexer_string();
  test_lexer_operators();
  test_lexer_delimiters();
  test_lexer_let();

  test_parser_stmt_let_int();
  test_parser_stmt_let_string();
  test_parser_stmt_let_bool();

  return 0;
}
