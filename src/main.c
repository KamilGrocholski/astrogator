#include <stdio.h>
#include <stdlib.h>

/* #include "compiler/ast/ast.h" */
#include "compiler/lexer/lexer.h"
#include "compiler/parser/parser.h"

int main() {
  char *input = "let var: int = 99; let a: int = 1;";
  Lexer *lexer = lexer_new(input);

  Parser *parser = parser_new(lexer);
  parser_parse_program(parser);

  return 0;
}
