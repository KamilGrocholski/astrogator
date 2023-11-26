#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

static void advance(Parser *p);
static void eat_or_error(Parser *p, TokenKind kind);
static void curr_must_be(Parser *p, TokenKind kind);
static void next_must_be(Parser *p, TokenKind kind);
static uint8_t is_curr_token(Parser *p, TokenKind kind);
static uint8_t is_next_token(Parser *p, TokenKind kind);
static void parse_value_by_type(Parser *p, Stmt *stmt, Exp *exp);

Parser *parser_new(Lexer *l) {
  Parser *p = malloc(sizeof(Parser));
  p->lexer = l;

  p->curr_token = lexer_get_next_token(l);
  p->next_token = lexer_get_next_token(l);

  if (p->curr_token) {
    p->tokens_count = 1;
  } else {
    p->tokens_count = 0;
  }

  return p;
}

Program *parser_parse_program(Parser *p) {
  Program *program = program_new();

  while (!is_curr_token(p, TOKEN_KIND_EOF)) {
    if (is_curr_token(p, TOKEN_KIND_LET)) {
      eat_or_error(p, TOKEN_KIND_LET);
      curr_must_be(p, TOKEN_KIND_IDENT);
      Stmt stmt;
      stmt.kind = STMT_KIND_LET;

      Exp name;
      name.kind = EXP_KIND_IDENT;
      name.u.ident = p->curr_token->literal;
      stmt.u.let.name = name;
      advance(p);
      eat_or_error(p, TOKEN_KIND_COLON);

      Exp value;
      parse_value_by_type(p, &stmt, &value);
      stmt.u.let.value = value;

      program_append(program, program_node_new(&stmt));
    }

    advance(p);
  }

  program_display(program);

  return program;
}

static void parse_value_by_type(Parser *p, Stmt *stmt, Exp *exp) {
  next_must_be(p, TOKEN_KIND_ASSIGN);

  // TODO sep functions, replace if else
  if (is_curr_token(p, TOKEN_KIND_VAL_TYPE_INT)) {
    exp->kind = EXP_KIND_INT;
    eat_or_error(p, TOKEN_KIND_VAL_TYPE_INT);
    stmt->u.let.value_type = VALUE_TYPE_INT;
    eat_or_error(p, TOKEN_KIND_ASSIGN);
    exp->u.int_value = atoi(p->curr_token->literal);
  } else if (is_curr_token(p, TOKEN_KIND_VAL_TYPE_STRING)) {
    exp->kind = EXP_KIND_STRING;
    eat_or_error(p, TOKEN_KIND_VAL_TYPE_STRING);
    stmt->u.let.value_type = VALUE_TYPE_STRING;
    eat_or_error(p, TOKEN_KIND_ASSIGN);
    is_curr_token(p, TOKEN_KIND_STRING);
    exp->u.string_value = p->curr_token->literal;
  } else if (is_curr_token(p, TOKEN_KIND_VAL_TYPE_BOOL)) {
    exp->kind = EXP_KIND_BOOL;
    eat_or_error(p, TOKEN_KIND_VAL_TYPE_BOOL);
    stmt->u.let.value_type = VALUE_TYPE_BOOL;
    eat_or_error(p, TOKEN_KIND_ASSIGN);

    if (is_curr_token(p, TOKEN_KIND_TRUE)) {
      exp->u.bool_value = TRUE;
    } else if (is_curr_token(p, TOKEN_KIND_FALSE)) {
      // TODO
      exp->u.bool_value = FALSE;
    } else {
      // TODO
      exp->u.bool_value = FALSE;
    }
  } else if (is_curr_token(p, TOKEN_KIND_L_BRACKET)) {
    exp->kind = EXP_KIND_ARRAY;
    eat_or_error(p, TOKEN_KIND_L_BRACKET);
    eat_or_error(p, TOKEN_KIND_R_BRACKET);
  }

  next_must_be(p, TOKEN_KIND_SEMICOLON);
}

static void advance(Parser *p) {
  p->curr_token = p->next_token;
  p->next_token = lexer_get_next_token(p->lexer);
  if (p->curr_token) {
    p->tokens_count++;
  }
}

static void eat_or_error(Parser *p, TokenKind kind) {
  /* assert(p->curr_token->kind == kind); */
  curr_must_be(p, kind);
  advance(p);
}

static uint8_t is_curr_token(Parser *p, TokenKind kind) {
  return p->curr_token->kind == kind;
}

static uint8_t is_next_token(Parser *p, TokenKind kind) {
  return p->next_token->kind == kind;
}

static void curr_must_be(Parser *p, TokenKind kind) {
  if (!is_curr_token(p, kind)) {
    printf(
        "Curr must be: `%s`, got: `%s` at position: %lu, tokens count: %lu, \n",
        get_token_name_by_val(kind), get_token_name_by_val(p->curr_token->kind),
        p->lexer->position, p->tokens_count);
  }
  /* assert(p->curr_token->kind == kind); */
}

static void next_must_be(Parser *p, TokenKind kind) {
  if (!is_next_token(p, kind)) {
    printf(
        "Next must be: `%s`, got: `%s` at position: %lu, tokens count: %lu, \n",
        get_token_name_by_val(kind), get_token_name_by_val(p->next_token->kind),
        p->lexer->position, p->tokens_count);
  }
  /* assert(p->next_token->kind == kind); */
}
