#ifndef _LEXER_TEST_H
#define _LEXER_TEST_H

#include "../../src/compiler/lexer.h"

typedef struct TokenTest {
  TokenKind kind;
  char *literal;
} TokenTest;

void test_token(Token *token, TokenKind expected_kind, char *expected_literal);
void test_tokens(char *name, char *input, TokenTest token_tests[],
                 size_t token_tests_len);

void test_token_free();
void test_lexer_free();

void test_lexer_string();
void test_lexer_ident();
void test_lexer_int();
void test_lexer_keywords();
void test_lexer_operators();
void test_lexer_delimiters();

void test_lexer_let();
void test_lexer_fn();

#endif // !_LEXER_TEST_H
