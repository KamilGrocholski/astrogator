#ifndef _LEXER_H
#define _LEXER_H

#include <stddef.h>

typedef enum {
  TOKEN_KIND_INT = 0,
  TOKEN_KIND_STRING = 1,
  TOKEN_KIND_IDENT = 2,

  // operators
  TOKEN_KIND_GT = 3,
  TOKEN_KIND_LT = 4,
  TOKEN_KIND_ASTERISK = 5,
  TOKEN_KIND_MINUS = 6,
  TOKEN_KIND_PLUS = 7,
  TOKEN_KIND_BANG = 8,
  TOKEN_KIND_QUESTION_MARK = 9,
  TOKEN_KIND_SLASH = 10,

  //
  TOKEN_KIND_EQUAL = 11,
  TOKEN_KIND_ASSIGN = 12,
  TOKEN_KIND_NOT_EQUAL = 13,

  // delimiters
  TOKEN_KIND_COMMA = 14,
  TOKEN_KIND_PERIOD = 15,
  TOKEN_KIND_COLON = 16,
  TOKEN_KIND_SEMICOLON = 17,

  // wrappers
  TOKEN_KIND_L_PAREN = 18,
  TOKEN_KIND_R_PAREN = 19,
  TOKEN_KIND_L_BRACKET = 20,
  TOKEN_KIND_R_BRACKET = 21,
  TOKEN_KIND_L_CURLY = 22,
  TOKEN_KIND_R_CURLY = 23,

  // keywords
  TOKEN_KIND_LET = 24,
  TOKEN_KIND_RETURN = 25,
  TOKEN_KIND_FUNCTION = 26,
  TOKEN_KIND_CONST = 27,
  TOKEN_KIND_USE = 28,
  // val types
  TOKEN_KIND_VAL_TYPE_INT = 29,

  TOKEN_KIND_ILLEGAL = 30,
  TOKEN_KIND_EOF = 31
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
