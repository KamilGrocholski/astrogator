#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "node.h"
#include "parser.h"

Stmt *parse_stmt_let(Parser *parser);
Stmt *parse_stmt_const(Parser *parser);
Stmt *parse_stmt_reassign(Parser *parser);
Stmt *parse_stmt_exp(Parser *parser);
Stmt *parse_stmt_block(Parser *parser);
Stmt *parse_stmt_if_else(Parser *parser);
Stmt *parse_stmt_return(Parser *parser);

Exp *parse_exp(Parser *parser);
Exp *parse_exp_normal(Parser *parser);
Exp *parse_exp_infix(Parser *parser, size_t precedence, Exp *left);
Exp *parse_exp_number(Parser *parser);
Exp *parse_exp_string(Parser *parser);
Exp *parse_exp_ident(Parser *parser);
Exp *parse_exp_fn(Parser *parser);
Exp *parse_exp_call(Parser *parser);
Exp *parse_exp_bool(Parser *parser);
Exp *parse_exp_nil(Parser *parser);
Exp *parse_exp_array(Parser *parser);
Exp *parse_exp_list(Parser *parser, TokenKind closing_token_kind);

void advance(Parser *parser);
void consume(Parser *parser, TokenKind expected_kind);

Stmt *parser_parse_stmt(Parser *parser) {
  switch (parser->curr_token.kind) {
  case TOKEN_LET:
    return parse_stmt_let(parser);
  case TOKEN_CONST:
    return parse_stmt_const(parser);
  case TOKEN_RETURN:
    return parse_stmt_return(parser);
  case TOKEN_IF:
    return parse_stmt_if_else(parser);
  default:
    if (parser->curr_token.kind == TOKEN_IDENT &&
        parser->next_token.kind == TOKEN_ASSIGN) {
      return parse_stmt_reassign(parser);
    }
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

Stmt *parse_stmt_let(Parser *parser) {
  consume(parser, TOKEN_LET);
  char *name = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);
  consume(parser, TOKEN_ASSIGN);
  Exp *value = parse_exp(parser);
  consume(parser, TOKEN_SEMICOLON);

  Stmt *stmt = stmt_let_new(name, value);
  return stmt;
}

Stmt *parse_stmt_const(Parser *parser) {
  consume(parser, TOKEN_CONST);
  char *name = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);
  consume(parser, TOKEN_ASSIGN);
  Exp *value = parse_exp(parser);
  consume(parser, TOKEN_SEMICOLON);

  Stmt *stmt = stmt_const_new(name, value);
  return stmt;
}

Stmt *parse_stmt_reassign(Parser *parser) {
  char *name = parser->curr_token.literal;
  consume(parser, TOKEN_IDENT);
  consume(parser, TOKEN_ASSIGN);
  Exp *value = parse_exp(parser);
  consume(parser, TOKEN_SEMICOLON);
  Stmt *reassign = stmt_reassign_new(name, value);
  return reassign;
}

Stmt *parse_stmt_if_else(Parser *parser) {
  consume(parser, TOKEN_IF);
  Exp *condition = parse_exp(parser);
  Stmt *consequence = parse_stmt_block(parser);
  Stmt *alternative = NULL;

  if (parser->curr_token.kind == TOKEN_ELSE) {
    consume(parser, TOKEN_ELSE);
    if (parser->curr_token.kind == TOKEN_IF) {
      alternative = parse_stmt_if_else(parser);
    } else {
      alternative = parse_stmt_block(parser);
    }
  }

  Stmt *if_else = stmt_if_else_new(condition, consequence, alternative);
  return if_else;
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
  return parse_exp_infix(parser, 0, parse_exp_normal(parser));

  printf("unhandled token kind in parse_exp %s\n",
         token_kind_to_str(parser->curr_token.kind));
  exit(1);
}

Exp *parse_exp_infix(Parser *parser, size_t precedence, Exp *left) {
  while (token_is_operator(parser->curr_token.kind) &&
         precedence < token_get_precedence(parser->curr_token.kind)) {
    TokenKind op_kind = parser->curr_token.kind;
    consume(parser, op_kind);

    Exp *right = parse_exp_normal(parser);
    while (token_is_operator(parser->curr_token.kind) &&
           precedence < token_get_precedence(parser->curr_token.kind)) {
      right = parse_exp_infix(
          parser, token_get_precedence(parser->curr_token.kind), right);
    }

    left = exp_infix_new(op_kind, left, right);
  }

  return left;
}

Exp *parse_exp_normal(Parser *parser) {
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
  case TOKEN_NIL:
    return parse_exp_nil(parser);
  default:
    printf("unhandled token kind in normal exp %s\n",
           token_kind_to_str(parser->next_token.kind));
    exit(1);
  }
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

  Exp *fn = exp_fn_new(name, params, body);
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

Exp *parse_exp_nil(Parser *parser) {
  consume(parser, TOKEN_NIL);
  return exp_nil_new();
}
