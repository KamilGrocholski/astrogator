#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static void _lexer_read_char(Lexer *lexer);
static void _lexer_skip_whitespace(Lexer *lexer);
static char *_lexer_read_string(Lexer *lexer);
static char *_lexer_read_int(Lexer *lexer);
static char *_lexer_read_ident(Lexer *lexer);
static TokenKind _get_token_kind_from_literal(const char *literal, size_t len);

static uint8_t _is_letter(char ch);
static char *_str_slice(char *src, size_t start, size_t end);

Lexer *lexer_new(char *input) {
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  lexer->input_len = strlen(input);
  lexer->input = input;
  lexer->position = 0;
  lexer->read_position = 0;

  _lexer_read_char(lexer);

  return lexer;
}

void lexer_free(Lexer *lexer) {
  if (lexer) {
    free(lexer);
  }

  lexer = NULL;
}

Token *token_new(TokenKind kind, char *literal) {
  Token *token = (Token *)malloc(sizeof(Token));
  token->kind = kind;
  token->literal = literal;

  return token;
}

void token_free(Token *token) {
  if (token) {
    free(token);
  }

  token = NULL;
}

void token_stringify(Token *token) {
  printf("kind: %d, literal: %s\n", token->kind, token->literal);
}

Token *lexer_get_next_token(Lexer *lexer) {
  _lexer_skip_whitespace(lexer);

  Token *token = NULL;

  switch (lexer->ch) {
  case ',':
    token = token_new(TOKEN_KIND_COMMA, ",");
    break;
  case '.':
    token = token_new(TOKEN_KIND_PERIOD, ".");
    break;
  case ':':
    token = token_new(TOKEN_KIND_COLON, ":");
    break;
  case ';':
    token = token_new(TOKEN_KIND_SEMICOLON, ";");
    break;
  case '(':
    token = token_new(TOKEN_KIND_L_PAREN, "(");
    break;
  case ')':
    token = token_new(TOKEN_KIND_R_PAREN, ")");
    break;
  case '[':
    token = token_new(TOKEN_KIND_L_BRACKET, "[");
    break;
  case ']':
    token = token_new(TOKEN_KIND_R_BRACKET, "]");
    break;
  case '{':
    token = token_new(TOKEN_KIND_L_CURLY, "{");
    break;
  case '}':
    token = token_new(TOKEN_KIND_R_CURLY, "}");
    break;
  case '!':
    token = token_new(TOKEN_KIND_BANG, "!");
    break;
  case '=':
    token = token_new(TOKEN_KIND_EQUAL, "=");
    break;
  case '-':
    token = token_new(TOKEN_KIND_MINUS, "-");
    break;
  case '+':
    token = token_new(TOKEN_KIND_PLUS, "+");
    break;
  case '*':
    token = token_new(TOKEN_KIND_ASTERISK, "*");
    break;
  case '"':
    token = token_new(TOKEN_KIND_STRING, _lexer_read_string(lexer));
    break;
  case '\0':
    token = token_new(TOKEN_KIND_EOF, "eof");
    break;
  }

  if (_is_letter(lexer->ch)) {
    char *ident_literal = _lexer_read_ident(lexer);
    size_t len = strlen(ident_literal);
    TokenKind kind = _get_token_kind_from_literal(ident_literal, len);
    token = token_new(kind, ident_literal);

    return token;
  } else if (isdigit(lexer->ch)) {
    char *int_literal = _lexer_read_int(lexer);
    token = token_new(TOKEN_KIND_INT, int_literal);

    return token;
  }

  if (!token) {
    token = token_new(TOKEN_KIND_ILLEGAL, &lexer->ch);
  }

  _lexer_read_char(lexer);

  return token;
}

static void _lexer_read_char(Lexer *lexer) {
  if (lexer->read_position >= lexer->input_len) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input[lexer->read_position];
  }

  lexer->position = lexer->read_position;
  lexer->read_position += 1;
}

static void _lexer_skip_whitespace(Lexer *lexer) {
  while (lexer->ch == ' ' || lexer->ch == '\r' || lexer->ch == '\n' ||
         lexer->ch == '\t') {
    _lexer_read_char(lexer);
  }
}

static char *_lexer_read_string(Lexer *lexer) {
  _lexer_read_char(lexer);

  size_t start = lexer->position;

  while (lexer->ch != '"') {
    if (lexer->ch == '\0') {
      break;
    }
    _lexer_read_char(lexer);
  }

  size_t end = lexer->position;

  return _str_slice(lexer->input, start, end);
}

static char *_lexer_read_int(Lexer *lexer) {
  size_t start = lexer->position;

  _lexer_read_char(lexer);

  while (isdigit(lexer->ch)) {
    _lexer_read_char(lexer);
  }

  size_t end = lexer->position;

  return _str_slice(lexer->input, start, end);
}

static char *_lexer_read_ident(Lexer *lexer) {
  size_t start = lexer->position;

  _lexer_read_char(lexer);

  while (_is_letter(lexer->ch)) {
    _lexer_read_char(lexer);
  }

  size_t end = lexer->position;

  return _str_slice(lexer->input, start, end);
}

static uint8_t _is_letter(char ch) {
  return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_');
}

static char *_str_slice(char *src, size_t start, size_t end) {
  size_t i;
  size_t size = (end - start);
  char *output = (char *)malloc(size * sizeof(char));

  for (i = 0; start < end; start++, i++) {
    output[i] = src[start];
  }

  output[size] = '\0';

  return output;
}

static TokenKind _get_token_kind_from_literal(const char *literal, size_t len) {
  if (strncmp(literal, "let", len) == 0) {
    return TOKEN_KIND_LET;
  } else if (strncmp(literal, "return", len) == 0) {
    return TOKEN_KIND_RETURN;
  } else if (strncmp(literal, "const", len) == 0) {
    return TOKEN_KIND_CONST;
  } else if (strncmp(literal, "use", len) == 0) {
    return TOKEN_KIND_USE;
  }

  return TOKEN_KIND_IDENT;
}
