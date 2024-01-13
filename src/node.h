#include <stdbool.h>
#include <stddef.h>

#include "lexer.h"

#ifndef _NODE_H
#define _NODE_H

typedef struct Exp Exp;
typedef struct Stmt Stmt;

typedef enum {
  STMT_LET,
  STMT_EXP,
  STMT_BLOCK,
  STMT_RETURN,
} StmtKind;

typedef enum {
  EXP_NUMBER,
  EXP_IDENT,
  EXP_STRING,
  EXP_LIST,
  EXP_ARRAY,
  EXP_FN,
  EXP_CALL,
  EXP_BOOL,
} ExpKind;

typedef struct Stmt {
  StmtKind kind;
  union {
    Exp *exp;

    struct {
      char *name;
      Exp *value;
    } let;

    struct {
      Stmt **stmts;
      size_t len;
      size_t cap;
    } block;

    struct {
      Exp *value;
    } ret;
  } data;
} Stmt;

typedef struct Exp {
  ExpKind kind;
  union {
    char *string;

    double number;

    bool boolean;

    struct {
      size_t len;
      size_t cap;
      struct Exp **elements;
    } list;

    struct {
      Exp *list;
    } array;

    struct {
      char *name;
      Exp *params;
      Stmt *body;
      Exp *ret;
    } fn;

    struct {
      char *name;
      Exp *args;
    } call;
  } data;
} Exp;

void stmt_print(Stmt *stmt);
char *stmt_kind_to_str(StmtKind kind);
Stmt *stmt_new();
Stmt *stmt_let_new(char *name, Exp *value);
Stmt *stmt_ret_new(Exp *value);
Stmt *stmt_exp_new(Exp *exp);
Stmt *stmt_block_new();
void stmt_block_append(Stmt *block, Stmt *inner_stmt);

void exp_print(Exp *node);
char *exp_kind_to_str(ExpKind kind);
Exp *exp_new();
Exp *exp_ident_new(char *ident);
Exp *exp_string_new(char *string);
Exp *exp_number_new(double value);
Exp *exp_fn_new(char *name, Exp *params, Stmt *body, Exp *ret);
Exp *exp_call_new(char *name, Exp *args);
Exp *exp_bool_new(bool boolean);
Exp *exp_array_new(Exp *list);
Exp *exp_list_new();
void exp_list_append(Exp *list, Exp *element);

typedef struct {
  Stmt **stmts;
  size_t len;
  size_t cap;
} Program;

Program *program_new();
void program_append(Program *program, Stmt *stmt);

#endif // !_NODE_H