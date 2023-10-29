#include <assert.h>
#include <string.h>

#include "../../src/compiler/lexer/lexer.h"
#include "lexer.test.h"

void test_token(Token *token, TokenKind expected_kind, char *expected_literal) {
  assert(token->kind == expected_kind);
  assert(strcmp(token->literal, expected_literal) == 0);
}

void test_lexer_string() {
  char *valid_input = "\"a string with whitespaces\"";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_STRING, "a string with whitespaces");

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_EOF, "eof");
}
