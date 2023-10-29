#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/compiler/lexer/lexer.h"
#include "lexer.test.h"

#define SHOULD_WITH_PRINT 1

static void _print_start_section(char *section) {
  if (SHOULD_WITH_PRINT) {
    printf("START TESTING: %s \n", section);
  }
}

static void _print_end_section(char *section) {
  if (SHOULD_WITH_PRINT) {
    printf("END TESTING: %s \n", section);
  }
}

void test_token(Token *token, TokenKind expected_kind, char *expected_literal) {
  if (SHOULD_WITH_PRINT) {
    printf("-- Token kind expected: |%d|, got: |%d| \n", expected_kind,
           token->kind);
  }
  assert(expected_kind == token->kind);
  if (SHOULD_WITH_PRINT) {
    printf("-- Token literal expected: |%s|, got: |%s| \n", expected_literal,
           token->literal);
  }
  assert(strcmp(expected_literal, token->literal) == 0);
}

void test_lexer_free() {
  char *valid_input = "input";
  Lexer *lexer = lexer_new(valid_input);

  lexer_free(lexer);
}

void test_token_free() {
  Token *token = token_new(TOKEN_KIND_RETURN, "return");

  token_free(token);
}

void test_lexer_string() {
  _print_start_section("string");
  char *valid_input = "\"a string with whitespaces\"";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_STRING, "a string with whitespaces");
  token_free(token1);

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_EOF, "eof");
  token_free(token2);

  lexer_free(lexer);
  _print_end_section("string");
}

void test_lexer_int() {
  _print_start_section("int");
  char *valid_input = " 1234  21234";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_INT, "1234");
  token_free(token1);

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_INT, "21234");
  token_free(token2);

  Token *token3 = lexer_get_next_token(lexer);
  test_token(token3, TOKEN_KIND_EOF, "eof");
  token_free(token3);

  lexer_free(lexer);
  _print_end_section("int");
}

void test_lexer_ident() {
  _print_start_section("ident");
  char *valid_input = "first_ident second_ident";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_IDENT, "first_ident");
  token_free(token1);

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_IDENT, "second_ident");
  token_free(token2);

  Token *token3 = lexer_get_next_token(lexer);
  test_token(token3, TOKEN_KIND_EOF, "eof");
  token_free(token3);

  lexer_free(lexer);
  _print_end_section("ident");
}

void test_lexer_keywords() {
  _print_start_section("keywords");
  char *valid_input = "let const return use";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_LET, "let");
  token_free(token1);

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_CONST, "const");
  token_free(token2);

  Token *token3 = lexer_get_next_token(lexer);
  test_token(token3, TOKEN_KIND_RETURN, "return");
  token_free(token3);

  Token *token4 = lexer_get_next_token(lexer);
  test_token(token4, TOKEN_KIND_USE, "use");
  token_free(token4);

  Token *token5 = lexer_get_next_token(lexer);
  test_token(token5, TOKEN_KIND_EOF, "eof");
  token_free(token5);

  lexer_free(lexer);
  _print_end_section("keywords");
}

void test_lexer_operators() {
  _print_start_section("operators");
  char *valid_input = "+ - * = ? ! != == < >";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_PLUS, "+");
  token_free(token1);

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_MINUS, "-");
  token_free(token2);

  Token *token3 = lexer_get_next_token(lexer);
  test_token(token3, TOKEN_KIND_ASTERISK, "*");
  token_free(token3);

  Token *token4 = lexer_get_next_token(lexer);
  test_token(token4, TOKEN_KIND_ASSIGN, "=");
  token_free(token4);

  Token *token5 = lexer_get_next_token(lexer);
  test_token(token5, TOKEN_KIND_QUESTION_MARK, "?");
  token_free(token5);

  Token *token6 = lexer_get_next_token(lexer);
  test_token(token6, TOKEN_KIND_BANG, "!");
  token_free(token6);

  Token *token7 = lexer_get_next_token(lexer);
  test_token(token7, TOKEN_KIND_NOT_EQUAL, "!=");
  token_free(token7);

  Token *token8 = lexer_get_next_token(lexer);
  test_token(token8, TOKEN_KIND_EQUAL, "==");
  token_free(token8);

  Token *token9 = lexer_get_next_token(lexer);
  test_token(token9, TOKEN_KIND_LT, "<");
  token_free(token9);

  Token *token10 = lexer_get_next_token(lexer);
  test_token(token10, TOKEN_KIND_GT, ">");
  token_free(token10);

  Token *token11 = lexer_get_next_token(lexer);
  test_token(token11, TOKEN_KIND_EOF, "eof");
  token_free(token11);

  lexer_free(lexer);
  _print_end_section("operators");
}

void test_lexer_delimiters() {
  _print_start_section("delimiters");
  char *valid_input = ", . ; :";
  Lexer *lexer = lexer_new(valid_input);

  Token *token1 = lexer_get_next_token(lexer);
  test_token(token1, TOKEN_KIND_COMMA, ",");
  token_free(token1);

  Token *token2 = lexer_get_next_token(lexer);
  test_token(token2, TOKEN_KIND_PERIOD, ".");
  token_free(token2);

  Token *token3 = lexer_get_next_token(lexer);
  test_token(token3, TOKEN_KIND_SEMICOLON, ";");
  token_free(token3);

  Token *token4 = lexer_get_next_token(lexer);
  test_token(token4, TOKEN_KIND_COLON, ":");
  token_free(token4);

  Token *token5 = lexer_get_next_token(lexer);
  test_token(token5, TOKEN_KIND_EOF, "eof");
  token_free(token5);

  lexer_free(lexer);
  _print_end_section("delimiters");
}
