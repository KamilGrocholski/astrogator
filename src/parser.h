#include "lexer.h"
#include "node.h"

#ifndef _PARSER_H
#define _PARSER_H

typedef struct {
  Lexer *lexer;
  Token curr_token;
  Token next_token;
} Parser;

Parser *parser_new(Lexer *lexer);
Stmt *parser_parse_stmt(Parser *parser);

#endif // !_PARSER_H
