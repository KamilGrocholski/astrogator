#include <stddef.h>

#ifndef _LEXER_H
#define _LEXER_H

typedef enum {
  TOKEN_EOF = 1,
  TOKEN_IDENT,
  TOKEN_INT,
  TOKEN_STRING,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LCURLY,
  TOKEN_RCURLY,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_COMMA,
  TOKEN_PLUS,
  TOKEN_LET,
  TOKEN_ILLEGAL,
  TOKEN_SEMICOLON,
  TOKEN_FN,
  TOKEN_RETURN,
  TOKEN_ASSIGN,
  TOKEN_ERROR,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_NIL,
} TokenKind;

typedef struct {
  TokenKind kind;
  char *literal;
} Token;

typedef struct {
  char *input;
  size_t input_len;
  size_t position;
  size_t read_position;
  char ch;
} Lexer;

void lexer_init(Lexer *lexer, char *input, size_t input_len);
void lexer_get_next_token(Lexer *lexer, Token *token);
char *token_kind_to_str(TokenKind kind);

#endif // !_LEXER_H
