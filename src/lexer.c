#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static void skip_whitespace(Lexer *lexer);
static void next_ch(Lexer *lexer);
static bool is_digit(char ch);
static bool is_alpha(char ch);
static void read_string(Lexer *lexer, Token *token);
static void read_int(Lexer *lexer, Token *token);
static void read_ident(Lexer *lexer, Token *token);
static TokenKind get_ident_kind_by_literal(char *literal);

// TODO convert to array
bool token_is_operator(TokenKind kind) {
  switch (kind) {
  case TOKEN_PLUS:
  case TOKEN_MINUS:
  case TOKEN_ASTERISK:
  case TOKEN_SLASH:
  case TOKEN_EQUAL:
  case TOKEN_NOT_EQUAL:
  case TOKEN_PERCENT:
    return true;
  default:
    return false;
  }
}

size_t token_get_precedence(TokenKind kind) {
  switch (kind) {
  case TOKEN_PLUS:
  case TOKEN_MINUS:
    return 1;
  case TOKEN_ASTERISK:
  case TOKEN_SLASH:
    return 2;
  case TOKEN_PERCENT:
    return 3;
  case TOKEN_EQUAL:
  case TOKEN_NOT_EQUAL:
    return 4;
  default:
    return 0;
  }
}

// TODO convert to array
char *token_kind_to_str(TokenKind kind) {
  switch (kind) {
  case TOKEN_LET:
    return "LET";
  case TOKEN_CONST:
    return "CONST";
  case TOKEN_DOT:
    return "DOT";
  case TOKEN_DOUBLE_DOT:
    return "DOUBLE_DOT";
  case TOKEN_INT:
    return "INT";
  case TOKEN_LPAREN:
    return "LPAREN";
  case TOKEN_RPAREN:
    return "RPAREN";
  case TOKEN_LBRACKET:
    return "LBRACKET";
  case TOKEN_RBRACKET:
    return "RBRACKET";
  case TOKEN_LCURLY:
    return "LCURLY";
  case TOKEN_RCURLY:
    return "RBRACKET";
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_PLUS:
    return "PLUS";
  case TOKEN_MINUS:
    return "MINUS";
  case TOKEN_ASTERISK:
    return "ASTERISK";
  case TOKEN_SLASH:
    return "SLASH";
  case TOKEN_PERCENT:
    return "PERCENT";
  case TOKEN_ERROR:
    return "ERROR";
  case TOKEN_IDENT:
    return "IDENT";
  case TOKEN_ILLEGAL:
    return "ILLEGAL";
  case TOKEN_SEMICOLON:
    return "SEMICOLON";
  case TOKEN_ASSIGN:
    return "ASSIGN";
  case TOKEN_EQUAL:
    return "EQUAL";
  case TOKEN_NOT_EQUAL:
    return "NOT_EQUAL";
  case TOKEN_BANG:
    return "BANG";
  case TOKEN_COMMA:
    return "COMMA";
  case TOKEN_FN:
    return "FN";
  case TOKEN_RETURN:
    return "RETURN";
  case TOKEN_TRUE:
    return "TRUE";
  case TOKEN_FALSE:
    return "FALSE";
  case TOKEN_NIL:
    return "NIL";
  case TOKEN_FOR:
    return "FOR";
  case TOKEN_RANGE:
    return "RANGE";
  case TOKEN_IN:
    return "IN";
  case TOKEN_IF:
    return "IF";
  case TOKEN_ELSE:
    return "ELSE";
  case TOKEN_EOF:
    return "EOF";
  }

  printf("unknown token kind %d\n", kind);
  exit(1);
}

void lexer_init(Lexer *lexer, char *input, size_t input_len) {
  if (lexer == NULL) {
    printf("Lexer is NULL\n");
    exit(1);
  }

  lexer->input = input;
  lexer->input_len = input_len;
  lexer->position = 0;
  lexer->read_position = 0;

  next_ch(lexer);
}

void lexer_get_next_token(Lexer *lexer, Token *token) {
  skip_whitespace(lexer);

  switch (lexer->ch) {
  case '"':
    read_string(lexer, token);
    return;
  case '%':
    token->kind = TOKEN_PERCENT;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '=':
    token->literal = NULL;
    next_ch(lexer);
    if (lexer->ch == '=') {
      token->kind = TOKEN_EQUAL;
      next_ch(lexer);
      return;
    }
    token->kind = TOKEN_ASSIGN;
    return;
  case '!':
    token->literal = NULL;
    next_ch(lexer);
    if (lexer->ch == '=') {
      next_ch(lexer);
      token->kind = TOKEN_NOT_EQUAL;
      return;
    }
    token->kind = TOKEN_BANG;
    return;
  case ';':
    token->kind = TOKEN_SEMICOLON;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '.':
    token->literal = NULL;
    next_ch(lexer);
    if (lexer->ch == '.') {
      token->kind = TOKEN_DOUBLE_DOT;
      next_ch(lexer);
      return;
    }
    token->kind = TOKEN_DOT;
    return;
  case '+':
    token->kind = TOKEN_PLUS;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '-':
    token->kind = TOKEN_MINUS;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '*':
    token->kind = TOKEN_ASTERISK;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '/':
    token->kind = TOKEN_SLASH;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '(':
    token->kind = TOKEN_LPAREN;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case ')':
    token->kind = TOKEN_RPAREN;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '{':
    token->kind = TOKEN_LCURLY;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '}':
    token->kind = TOKEN_RCURLY;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '[':
    token->kind = TOKEN_LBRACKET;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case ']':
    token->kind = TOKEN_RBRACKET;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case ',':
    token->kind = TOKEN_COMMA;
    token->literal = NULL;
    next_ch(lexer);
    return;
  case '\0':
    token->kind = TOKEN_EOF;
    token->literal = NULL;
    return;
  }

  if (is_digit(lexer->ch)) {
    read_int(lexer, token);
    return;
  } else if (is_alpha(lexer->ch)) {
    read_ident(lexer, token);
    return;
  }

  token->kind = TOKEN_ILLEGAL;
  token->literal = NULL;
  next_ch(lexer);
}

static void skip_whitespace(Lexer *lexer) {
  while (isspace(lexer->ch)) {
    next_ch(lexer);
  }
}

static void next_ch(Lexer *lexer) {
  if (lexer->read_position >= lexer->input_len) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input[lexer->read_position];
  }
  lexer->position = lexer->read_position;
  lexer->read_position++;
}

static void read_int(Lexer *lexer, Token *token) {
  size_t start = lexer->position;
  while (is_digit(lexer->ch)) {
    next_ch(lexer);
  }
  size_t length = lexer->position - start;
  char *literal = strndup(&lexer->input[start], length);

  token->kind = TOKEN_INT;
  token->literal = literal;
}

static void read_ident(Lexer *lexer, Token *token) {
  size_t start = lexer->position;
  while (is_alpha(lexer->ch)) {
    next_ch(lexer);
  }
  size_t length = lexer->position - start;
  char *literal = strndup(&lexer->input[start], length);

  token->kind = get_ident_kind_by_literal(literal);
  token->literal = literal;
}

static void read_string(Lexer *lexer, Token *token) {
  next_ch(lexer);
  size_t start = lexer->position;
  while (lexer->ch != '"') {
    if (lexer->ch == '\0') {
      break;
    }
    next_ch(lexer);
  }
  size_t length = lexer->position - start;
  next_ch(lexer);
  char *literal = strndup(&lexer->input[start], length);

  token->kind = TOKEN_STRING;
  token->literal = literal;
}

static bool is_digit(char ch) { return ch >= '0' && ch <= '9'; }
static bool is_alpha(char ch) {
  return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_';
}

static TokenKind get_ident_kind_by_literal(char *literal) {
  if (strcmp(literal, "let") == 0) {
    return TOKEN_LET;
  } else if (strcmp(literal, "const") == 0) {
    return TOKEN_CONST;
  } else if (strcmp(literal, "return") == 0) {
    return TOKEN_RETURN;
  } else if (strcmp(literal, "fn") == 0) {
    return TOKEN_FN;
  } else if (strcmp(literal, "true") == 0) {
    return TOKEN_TRUE;
  } else if (strcmp(literal, "false") == 0) {
    return TOKEN_FALSE;
  } else if (strcmp(literal, "nil") == 0) {
    return TOKEN_NIL;
  } else if (strcmp(literal, "for") == 0) {
    return TOKEN_FOR;
  } else if (strcmp(literal, "range") == 0) {
    return TOKEN_RANGE;
  } else if (strcmp(literal, "in") == 0) {
    return TOKEN_IN;
  } else if (strcmp(literal, "if") == 0) {
    return TOKEN_IF;
  } else if (strcmp(literal, "else") == 0) {
    return TOKEN_ELSE;
  } else {
    return TOKEN_IDENT;
  }
}
