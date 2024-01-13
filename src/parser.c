#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "node.h"
#include "parser.h"

Stmt *parse_stmt_let(Parser *parser);
Stmt *parse_stmt_exp(Parser *parser);
Stmt *parse_stmt_block(Parser *parser);
Stmt *parse_stmt_return(Parser *parser);

Exp *parse_exp(Parser *parser);
Exp *parse_exp_number(Parser *parser);
Exp *parse_exp_string(Parser *parser);
Exp *parse_exp_ident(Parser *parser);
Exp *parse_exp_fn(Parser *parser);
Exp *parse_exp_call(Parser *parser);
Exp *parse_exp_bool(Parser *parser);
Exp *parse_exp_array(Parser *parser);
Exp *parse_exp_list(Parser *parser, TokenKind closing_token_kind);

void advance(Parser *parser);
void consume(Parser *parser, TokenKind expected_kind);

Stmt *parser_parse_stmt(Parser *parser) {
  switch (parser->curr_token.kind) {
  case TOKEN_LET:
    return parse_stmt_let(parser);
  case TOKEN_RETURN:
    return parse_stmt_return(parser);
  default:
    return parse_stmt_exp(parser);
  }
}

Parser *parser_new(Lexer *lexer) {
  Parser *parser = (Parser *)malloc(sizeof(Parser));
  if (parser == NULL) {
    printf("parser failed memory\n");
    exit(1);
  }
  parser->lexer = lexer;
  lexer_get_next_token(parser->lexer, &parser->curr_token);
  lexer_get_next_token(parser->lexer, &parser->next_token);
  return parser;
}

void advance(Parser *parser) {
  parser->curr_token = parser->next_token;
  lexer_get_next_token(parser->lexer, &parser->next_token);
}

void consume(Parser *parser, TokenKind expected_kind) {
  if (parser->curr_token.kind == expected_kind) {
    advance(parser);
  } else {
    printf("Parse consume error: expected %s, got %s\n",
           token_kind_to_str(expected_kind),
           token_kind_to_str(parser->curr_token.kind));
    exit(1);
  }
}

void expect_curr(Parser *parser, TokenKind kind) {
  if (parser->curr_token.kind != kind) {
    printf("Parse expect_curr error: expected %s, got %s\n",
           token_kind_to_str(kind), token_kind_to_str(parser->curr_token.kind));
    exit(1);
  }
};

Stmt *parse_stmt_let(Parser *parser) {
  consume(parser, TOKEN_LET);

  expect_curr(parser, TOKEN_IDENT);
  char *name = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);
  consume(parser, TOKEN_ASSIGN);
  Exp *value = parse_exp(parser);
  consume(parser, TOKEN_SEMICOLON);

  Stmt *stmt = stmt_let_new(name, value);

  return stmt;
}

Stmt *parse_stmt_return(Parser *parser) {
  consume(parser, TOKEN_RETURN);
  Exp *value = parse_exp(parser);
  consume(parser, TOKEN_SEMICOLON);
  Stmt *ret = stmt_ret_new(value);

  return ret;
}

Stmt *parse_stmt_block(Parser *parser) {
  consume(parser, TOKEN_LCURLY);
  Stmt *block = stmt_block_new();

  while (parser->curr_token.kind != TOKEN_RCURLY) {
    Stmt *stmt = parser_parse_stmt(parser);
    stmt_block_append(block, stmt);
  }

  consume(parser, TOKEN_RCURLY);

  return block;
}

Stmt *parse_stmt_exp(Parser *parser) {
  Exp *exp = parse_exp(parser);
  Stmt *stmt = stmt_exp_new(exp);
  if (parser->curr_token.kind == TOKEN_SEMICOLON) {
    advance(parser);
  }
  return stmt;
}

Exp *parse_exp(Parser *parser) {
  while (parser->curr_token.kind != TOKEN_SEMICOLON) {
    switch (parser->curr_token.kind) {
    case TOKEN_INT:
      return parse_exp_number(parser);
    case TOKEN_IDENT:
      if (parser->next_token.kind == TOKEN_LPAREN) {
        return parse_exp_call(parser);
      }
      return parse_exp_ident(parser);
    case TOKEN_STRING:
      return parse_exp_string(parser);
    case TOKEN_LBRACKET:
      return parse_exp_array(parser);
    case TOKEN_FN:
      return parse_exp_fn(parser);
    case TOKEN_TRUE:
    case TOKEN_FALSE:
      return parse_exp_bool(parser);
    }
  }

  printf("unhandled token kind in parse_exp %s\n",
         token_kind_to_str(parser->curr_token.kind));
  exit(1);
}

Exp *parse_exp_number(Parser *parser) {
  double value = atof(parser->curr_token.literal);
  consume(parser, TOKEN_INT);
  Exp *exp = exp_number_new(value);
  return exp;
}

Exp *parse_exp_string(Parser *parser) {
  char *string = parser->curr_token.literal;
  consume(parser, TOKEN_STRING);
  Exp *exp = exp_string_new(string);
  return exp;
}

Exp *parse_exp_ident(Parser *parser) {
  char *ident = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);
  Exp *exp = exp_ident_new(ident);
  return exp;
}

Exp *parse_exp_list(Parser *parser, TokenKind closing_token_kind) {
  Exp *list = exp_list_new();

  if (parser->curr_token.kind == closing_token_kind) {
    return list;
  }

  Exp *first = parse_exp(parser);
  exp_list_append(list, first);

  while (parser->curr_token.kind == TOKEN_COMMA) {
    consume(parser, TOKEN_COMMA);
    Exp *element = parse_exp(parser);
    exp_list_append(list, element);
  }

  return list;
}

Exp *parse_exp_array(Parser *parser) {
  consume(parser, TOKEN_LBRACKET);
  Exp *list = parse_exp_list(parser, TOKEN_RBRACKET);
  Exp *array = exp_array_new(list);
  consume(parser, TOKEN_RBRACKET);

  return array;
}

Exp *parse_exp_fn(Parser *parser) {
  consume(parser, TOKEN_FN);

  char *name = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);

  consume(parser, TOKEN_LPAREN);
  Exp *params = parse_exp_list(parser, TOKEN_RPAREN);
  consume(parser, TOKEN_RPAREN);
  Stmt *body = parse_stmt_block(parser);
  Exp *ret = exp_new();

  Exp *fn = exp_fn_new(name, params, body, NULL);
  return fn;
}

Exp *parse_exp_call(Parser *parser) {
  char *name = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);
  consume(parser, TOKEN_LPAREN);
  Exp *args = parse_exp_list(parser, TOKEN_RPAREN);
  consume(parser, TOKEN_RPAREN);
  Exp *call = exp_call_new(name, args);
  return call;
}

Exp *parse_exp_bool(Parser *parser) {
  if (parser->curr_token.kind == TOKEN_TRUE) {
    consume(parser, TOKEN_TRUE);
    return exp_bool_new(true);
  } else {
    consume(parser, TOKEN_FALSE);
    return exp_bool_new(false);
  }
}
