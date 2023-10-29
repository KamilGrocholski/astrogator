#ifndef _LEXER_TEST_H
#define _LEXER_TEST_H

#include "../../src/compiler/lexer/lexer.h"

void test_lexer_string();
void test_token(Token *token, TokenKind expected_kind, char *expected_literal);

#endif // !_LEXER_TEST_H
