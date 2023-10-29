#ifndef _LEXER_H
#define _LEXER_H

#include <stddef.h>

typedef enum {
  TOKEN_KIND_INT = 0,
  TOKEN_KIND_STRING,
  TOKEN_KIND_IDENT,

  TOKEN_KIND_COMMA,
  TOKEN_KIND_PERIOD,
  TOKEN_KIND_COLON,
  TOKEN_KIND_SEMICOLON,
  TOKEN_KIND_ASTERISK,
  TOKEN_KIND_BANG,
  TOKEN_KIND_EQUAL,
  TOKEN_KIND_MINUS,
  TOKEN_KIND_PLUS,

  TOKEN_KIND_L_PAREN,
  TOKEN_KIND_R_PAREN,
  TOKEN_KIND_L_BRACKET,
  TOKEN_KIND_R_BRACKET,
  TOKEN_KIND_L_CURLY,
  TOKEN_KIND_R_CURLY,

  // keywords
  TOKEN_KIND_LET,
  TOKEN_KIND_RETURN,
  TOKEN_KIND_FUNCTION,
  TOKEN_KIND_CONST,
  TOKEN_KIND_USE,

  TOKEN_KIND_ILLEGAL,
  TOKEN_KIND_EOF
} TokenKind;

typedef struct {
  TokenKind kind;
  char *literal;
} Token;

typedef struct {
  char *input;
  size_t input_len;
  char ch;
  size_t position;
  size_t read_position;
} Lexer;

Lexer *lexer_new(char *input);
void lexer_free(Lexer *lexer);

Token *token_new(TokenKind kind, char *literal);
void token_free(Token *token);
void token_stringify(Token *token);

Token *lexer_get_next_token(Lexer *lexer);

#endif // !_LEXER_H
