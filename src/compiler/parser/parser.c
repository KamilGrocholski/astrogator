#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "parser.h"

static void _parser_advance(Parser *parser);
static void _parser_eat(Parser *parser, TokenKind expected_kind);

static void _parser_assert_curr_token(Parser *parser, TokenKind expected_kind);
static void _parser_assert_next_token(Parser *parser, TokenKind expected_kind);

static Stmt *_parser_parse_stmt(Parser *parser);

Parser *parser_new(Lexer *lexer) {
  Parser *parser = malloc(sizeof(Parser));
  parser->lexer = lexer;
  parser->current_token = lexer_get_next_token(lexer);
  parser->next_token = lexer_get_next_token(lexer);

  return parser;
}

void parser_free(Parser *parser) {
  if (parser) {
    lexer_free(parser->lexer);
    token_free(parser->current_token);
    token_free(parser->next_token);
    free(parser);
    // TODO
  }

  parser = NULL;
}

Program *parser_parse_program(Parser *parser) {
  Program *program = malloc(sizeof(Program));
  program->stmts_len = 0;
  Stmt *stmts = malloc(sizeof(Stmt) * 256);
  program->stmts = stmts;

  while (parser->current_token->kind != TOKEN_KIND_EOF) {
    /* printf("before stmt: %s\n", parser->current_token->literal); */
    Stmt *stmt = _parser_parse_stmt(parser);
    if (stmt) {
      stmts[program->stmts_len] = *stmt;
      program->stmts_len++;
    }
    _parser_advance(parser);
  }

  return program;
}

static Stmt *_parser_parse_stmt(Parser *parser) {
  TokenKind kind = parser->current_token->kind;

  if (kind == TOKEN_KIND_LET) {
    _parser_advance(parser);
    Stmt *stmt = malloc(sizeof(Stmt)); // TODO new, free
    stmt->kind = STMT_KIND_LET;

    // TODO

    _parser_assert_curr_token(parser, TOKEN_KIND_IDENT);
    stmt->sb.let_decl_assign.ident = *parser->current_token->literal;
    _parser_advance(parser);
    _parser_eat(parser, TOKEN_KIND_COLON);
    _parser_assert_curr_token(parser, TOKEN_KIND_VAL_TYPE_INT);
    stmt->sb.let_decl_assign.type = VAL_TYPE_INT; // TODO add value type parser
    _parser_advance(parser);
    _parser_eat(parser, TOKEN_KIND_ASSIGN);
    Exp *exp = malloc(sizeof(Exp));
    exp->kind = EXP_KIND_INTEGER_LIT;
    exp->eb.integer_lit = atoi(parser->current_token->literal);
    stmt->sb.let_decl_assign.value = exp;
    _parser_assert_next_token(parser, TOKEN_KIND_SEMICOLON);

    return stmt;
  } else if (kind == TOKEN_KIND_RETURN) {
    _parser_eat(parser, TOKEN_KIND_RETURN);
    Stmt *stmt = malloc(sizeof(Stmt));
    stmt->kind = STMT_KIND_RETURN;

    // TODO

    return stmt;
  } else {
    return NULL;
  }
}

static void _parser_advance(Parser *parser) {
  parser->current_token = parser->next_token;
  /* printf("curr: %s\n", parser->current_token->literal); */
  parser->next_token = lexer_get_next_token(parser->lexer);
  /* printf("next: %s\n", parser->next_token->literal); */
}

static void _parser_eat(Parser *parser, TokenKind expected_kind) {
  assert(parser->current_token->kind == expected_kind);
  _parser_advance(parser);
}

static void _parser_assert_curr_token(Parser *parser, TokenKind expected_kind) {
  assert(parser->current_token->kind == expected_kind);
}

static void _parser_assert_next_token(Parser *parser, TokenKind expected_kind) {
  assert(parser->next_token->kind == expected_kind);
}
