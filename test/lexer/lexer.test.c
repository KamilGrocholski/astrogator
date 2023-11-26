#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../src/compiler/lexer.h"
#include "lexer.test.h"

#define SHOULD_WITH_PRINT 1

static void _print_start_section(char *section);

static void _print_end_section(char *section);

void test_token(Token *token, TokenKind expected_kind, char *expected_literal) {
  if (SHOULD_WITH_PRINT) {
    printf("-- Token kind expected: |%d|, got: |%d| \n", expected_kind,
           token->kind);
  }
  if (SHOULD_WITH_PRINT) {
    printf("-- Token literal expected: |%s|, got: |%s| \n", expected_literal,
           token->literal);
  }
  assert(expected_kind == token->kind);
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

void test_tokens(char *name, char *input, TokenTest token_tests[],
                 size_t token_tests_len) {
  _print_start_section(name);
  Lexer *lexer = lexer_new(input);
  Token *token = NULL;

  for (size_t i = 0; i < token_tests_len; i++) {
    token = lexer_get_next_token(lexer);
    test_token(token, token_tests[i].kind, token_tests[i].literal);
    token_free(token);
  }
  _print_end_section(name);
}

void test_lexer_string() {
  char *input = "\"a string with whitespaces\"";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_STRING, "a string with whitespaces"},
      {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("string", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_int() {
  char *input = " 1234  21234";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_INT, "1234"},
      {TOKEN_KIND_INT, "21234"},
      {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("ident", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_ident() {
  char *input = "first_ident second_ident";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_IDENT, "first_ident"},
      {TOKEN_KIND_IDENT, "second_ident"},
      {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("ident", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_keywords() {
  char *input = "let const return use int string bool true false fn";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_LET, "let"},
      {TOKEN_KIND_CONST, "const"},
      {TOKEN_KIND_RETURN, "return"},
      {TOKEN_KIND_USE, "use"},
      {TOKEN_KIND_VAL_TYPE_INT, "int"},
      {TOKEN_KIND_VAL_TYPE_STRING, "string"},
      {TOKEN_KIND_VAL_TYPE_BOOL, "bool"},
      {TOKEN_KIND_TRUE, "true"},
      {TOKEN_KIND_FALSE, "false"},
      {TOKEN_KIND_FUNCTION, "fn"},
      {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("keywords", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_operators() {
  char *input = "+ - * = ? ! != == < >";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_PLUS, "+"},
      {TOKEN_KIND_MINUS, "-"},
      {TOKEN_KIND_ASTERISK, "*"},
      {TOKEN_KIND_ASSIGN, "="},
      {TOKEN_KIND_QUESTION_MARK, "?"},
      {TOKEN_KIND_BANG, "!"},
      {TOKEN_KIND_NOT_EQUAL, "!="},
      {TOKEN_KIND_EQUAL, "=="},
      {TOKEN_KIND_LT, "<"},
      {TOKEN_KIND_GT, ">"},
      {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("operators", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_delimiters() {
  char *input = ", . ; :";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_COMMA, ","},     {TOKEN_KIND_PERIOD, "."},
      {TOKEN_KIND_SEMICOLON, ";"}, {TOKEN_KIND_COLON, ":"},
      {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("delimiters", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_let() {
  char *input = "let var: int = 234;";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_LET, "let"},     {TOKEN_KIND_IDENT, "var"},
      {TOKEN_KIND_COLON, ":"},     {TOKEN_KIND_VAL_TYPE_INT, "int"},
      {TOKEN_KIND_ASSIGN, "="},    {TOKEN_KIND_INT, "234"},
      {TOKEN_KIND_SEMICOLON, ";"}, {TOKEN_KIND_EOF, "eof"},
  };
  test_tokens("let", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

void test_lexer_fn() {
  char *input = "fn add(a: int, b: int): int {return a + b;}";
  struct TokenTest token_tests[] = {
      {TOKEN_KIND_FUNCTION, "fn"},      {TOKEN_KIND_IDENT, "add"},
      {TOKEN_KIND_L_PAREN, "("},        {TOKEN_KIND_IDENT, "a"},
      {TOKEN_KIND_COLON, ":"},          {TOKEN_KIND_VAL_TYPE_INT, "int"},
      {TOKEN_KIND_COMMA, ","},          {TOKEN_KIND_IDENT, "b"},
      {TOKEN_KIND_COLON, ":"},          {TOKEN_KIND_VAL_TYPE_INT, "int"},
      {TOKEN_KIND_R_PAREN, ")"},        {TOKEN_KIND_COLON, ":"},
      {TOKEN_KIND_VAL_TYPE_INT, "int"}, {TOKEN_KIND_L_CURLY, "{"},
      {TOKEN_KIND_RETURN, "return"},    {TOKEN_KIND_IDENT, "a"},
      {TOKEN_KIND_PLUS, "+"},           {TOKEN_KIND_IDENT, "b"},
      {TOKEN_KIND_SEMICOLON, ";"},      {TOKEN_KIND_R_CURLY, "}"},
      {TOKEN_KIND_EOF, "eof"}};
  test_tokens("fn", input, token_tests,
              sizeof(token_tests) / sizeof(token_tests[0]));
}

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
