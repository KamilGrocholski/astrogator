#ifndef _PARSER_H
#define _PARSER_H

#include <stdint.h>

#include "lexer.h"

#define TRUE 1;
#define FALSE 0;

typedef enum ValueType {
  VALUE_TYPE_INT,
  VALUE_TYPE_STRING,
  VALUE_TYPE_BOOL,
} ValueType;

typedef enum ExpKind {
  EXP_KIND_INT,
  EXP_KIND_STRING,
  EXP_KIND_BOOL,
  EXP_KIND_IDENT,
} ExpKind;

typedef struct Exp {
  ExpKind kind;
  union {
    int int_value;
    char *string_value;
    char *ident;
    uint8_t bool_value;
  } u;
} Exp;

typedef enum StmtKind {
  STMT_KIND_LET,
} StmtKind;

typedef struct Stmt {
  StmtKind kind;
  union {
    struct {
      Exp name;
      ValueType value_type;
      Exp value;
    } let;
  } u;
} Stmt;

typedef struct Parser {
  Lexer *lexer;
  Token *curr_token;
  Token *next_token;
  size_t tokens_count;
} Parser;

typedef struct ProgramNode {
  Stmt *stmt;
  struct ProgramNode *next;
} ProgramNode;

typedef struct Program {
  struct ProgramNode *head;
  struct ProgramNode *tail;
} Program;

Program *program_new();
/* void program_free(Program *p); */
ProgramNode *program_node_new(Stmt *stmt);
/* void program_node_free(ProgramNode *n); */
void program_append(Program *p, ProgramNode *node);
void program_display(Program *program);

Program *parser_parse_program(Parser *p);
Parser *parser_new(Lexer *l);
/* void parser_free(Parser *p); */

#endif // !_PARSER_H
