#include <stdio.h>
#include <stdlib.h>

/* #include "compiler/ast/ast.h" */
#include "compiler/lexer/lexer.h"

int main() {
  char *input = "\"siema\" 52134; use var = 2;";
  Lexer *lexer = lexer_new(input);

  Token *token1 = lexer_get_next_token(lexer);
  Token *token2 = lexer_get_next_token(lexer);
  Token *token3 = lexer_get_next_token(lexer);
  Token *token4 = lexer_get_next_token(lexer);
  Token *token5 = lexer_get_next_token(lexer);
  Token *token6 = lexer_get_next_token(lexer);
  Token *token7 = lexer_get_next_token(lexer);
  Token *token8 = lexer_get_next_token(lexer);

  token_stringify(token1);
  token_stringify(token2);
  token_stringify(token3);
  token_stringify(token4);
  token_stringify(token5);
  token_stringify(token6);
  token_stringify(token7);
  token_stringify(token8);

  /* AST *program = ast_new((AST){ */
  /*     AST_INT8, */
  /*     {.AST_ADD = (struct AST_ADD){ */
  /*          ast_new((AST){AST_INT8, {.AST_INT8 = (struct AST_INT8){127}}}), */
  /*          ast_new((AST){AST_INT8, {.AST_INT8 = (struct AST_INT8){-128}}}),
   */
  /*      }}}); */

  return 0;
}
