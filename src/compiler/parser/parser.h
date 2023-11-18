#ifndef _PARSER_H
#define _PARSER_H

#include "../lexer/lexer.h"

typedef struct Stmt Stmt;
typedef struct Exp Exp;

typedef enum ExpKind {
  EXP_KIND_IDENT,
  EXP_KIND_STRING_LIT,
  EXP_KIND_INTEGER_LIT
} ExpKind;

typedef enum StmtKind {
  STMT_KIND_LET,
  STMT_KIND_RETURN,
  STMT_KIND_EXPRESSION
} StmtKind;

typedef enum ValueType {
  VAL_TYPE_NULL,
  VAL_TYPE_INT,
  VAL_TYPE_STRING,
} ValueType;

typedef struct Exp {
  ExpKind kind;
  union {
    int integer_lit;
    char string_lit;
    char ident;
  } eb;
} Exp;

typedef struct Stmt {
  StmtKind kind;
  union {
    Exp *exp;
    struct {
      ValueType type;
      char ident;
      Exp *value;
    } let_decl_assign;
  } sb;
} Stmt;

typedef struct Parser {
  Lexer *lexer;
  Token *current_token;
  Token *next_token;
} Parser;

typedef struct Program {
  size_t stmts_len;
  Stmt *stmts;
} Program;

Parser *parser_new(Lexer *lexer);
void parser_free(Parser *parser);
Program *parser_parse_program(Parser *parser);
// TODO program

#endif // !_PARSER_H
