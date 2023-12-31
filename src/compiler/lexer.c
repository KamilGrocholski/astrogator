#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static void read_char(Lexer *lexer);
static char peek_char(Lexer *lexer);
static void skip_whitespace(Lexer *lexer);
static char *read_string(Lexer *lexer);
static char *read_int(Lexer *lexer);
static char *read_ident(Lexer *lexer);
static TokenKind get_token_kind_from_literal(const char *literal, size_t len);

static uint8_t is_alpha(char ch);
static char *str_slice(char *src, size_t start, size_t end);

Lexer *lexer_new(char *input) {
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  lexer->input_len = strlen(input);
  lexer->input = input;
  lexer->position = 0;
  lexer->read_position = 0;

  read_char(lexer);

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
  skip_whitespace(lexer);

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
    if (peek_char(lexer) == '=') {
      read_char(lexer);
      token = token_new(TOKEN_KIND_NOT_EQUAL, "!=");
    } else {
      token = token_new(TOKEN_KIND_BANG, "!");
    }
    break;
  case '?':
    token = token_new(TOKEN_KIND_QUESTION_MARK, "?");
    break;
  case '=':
    if (peek_char(lexer) == '=') {
      read_char(lexer);
      token = token_new(TOKEN_KIND_EQUAL, "==");
    } else {
      token = token_new(TOKEN_KIND_ASSIGN, "=");
    }
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
  case '>':
    token = token_new(TOKEN_KIND_GT, ">");
    break;
  case '<':
    token = token_new(TOKEN_KIND_LT, "<");
    break;
  case '"':
    token = token_new(TOKEN_KIND_STRING, read_string(lexer));
    break;
  case '\0':
    token = token_new(TOKEN_KIND_EOF, "eof");
    break;
  }

  if (is_alpha(lexer->ch)) {
    char *ident_literal = read_ident(lexer);
    size_t len = strlen(ident_literal);
    TokenKind kind = get_token_kind_from_literal(ident_literal, len);
    token = token_new(kind, ident_literal);

    return token;
  } else if (isdigit(lexer->ch)) {
    char *int_literal = read_int(lexer);
    token = token_new(TOKEN_KIND_INT, int_literal);

    return token;
  }

  if (!token) {
    token = token_new(TOKEN_KIND_ILLEGAL, &lexer->ch);
  }

  read_char(lexer);

  return token;
}

static void read_char(Lexer *lexer) {
  if (lexer->read_position >= lexer->input_len) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input[lexer->read_position];
  }

  lexer->position = lexer->read_position;
  lexer->read_position += 1;
}

static char peek_char(Lexer *lexer) {
  if (lexer->read_position >= lexer->input_len) {
    return '\0';
  } else {
    return lexer->input[lexer->read_position];
  }
}

static void skip_whitespace(Lexer *lexer) {
  while (lexer->ch == ' ' || lexer->ch == '\r' || lexer->ch == '\n' ||
         lexer->ch == '\t') {
    read_char(lexer);
  }
}

static char *read_string(Lexer *lexer) {
  read_char(lexer);

  size_t start = lexer->position;

  while (lexer->ch != '"') {
    if (lexer->ch == '\0') {
      break;
    }
    read_char(lexer);
  }

  size_t end = lexer->position;

  return str_slice(lexer->input, start, end);
}

static char *read_int(Lexer *lexer) {
  size_t start = lexer->position;

  read_char(lexer);

  while (isdigit(lexer->ch)) {
    read_char(lexer);
  }

  size_t end = lexer->position;

  return str_slice(lexer->input, start, end);
}

static char *read_ident(Lexer *lexer) {
  size_t start = lexer->position;

  read_char(lexer);

  while (is_alpha(lexer->ch)) {
    read_char(lexer);
  }

  size_t end = lexer->position;

  return str_slice(lexer->input, start, end);
}

static uint8_t is_alpha(char ch) {
  return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_');
}

static char *str_slice(char *src, size_t start, size_t end) {
  size_t i;
  size_t size = (end - start);
  char *output = (char *)malloc(size * sizeof(char));

  for (i = 0; start < end; start++, i++) {
    output[i] = src[start];
  }

  output[size] = '\0';

  return output;
}

static TokenKind get_token_kind_from_literal(const char *literal, size_t len) {
  if (strncmp(literal, "let", len) == 0) {
    return TOKEN_KIND_LET;
  } else if (strncmp(literal, "return", len) == 0) {
    return TOKEN_KIND_RETURN;
  } else if (strncmp(literal, "fn", len) == 0) {
    return TOKEN_KIND_FUNCTION;
  } else if (strncmp(literal, "const", len) == 0) {
    return TOKEN_KIND_CONST;
  } else if (strncmp(literal, "use", len) == 0) {
    return TOKEN_KIND_USE;
  } else if (strncmp(literal, "true", len) == 0) {
    return TOKEN_KIND_TRUE;
  } else if (strncmp(literal, "false", len) == 0) {
    return TOKEN_KIND_FALSE;
  } else if (strncmp(literal, "int", len) == 0) {
    return TOKEN_KIND_VAL_TYPE_INT;
  } else if (strncmp(literal, "string", len) == 0) {
    return TOKEN_KIND_VAL_TYPE_STRING;
  } else if (strcmp(literal, "bool") ==
             0) { // TODO why strncmp doesnt work here, it treats "b" as "bool"
                  // and so on
    return TOKEN_KIND_VAL_TYPE_BOOL;
  }

  return TOKEN_KIND_IDENT;
}

const char *get_token_name_by_val(TokenKind kind) { return TokensNames[kind]; }
