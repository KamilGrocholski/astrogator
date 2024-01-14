#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

Program *program_new() {
  Program *program = (Program *)malloc(sizeof(Program));
  if (program == NULL) {
    printf("program failed memory\n");
    exit(1);
  }
  program->len = 0;
  program->cap = 10;
  program->stmts = (Stmt **)malloc(sizeof(Stmt *) * program->cap);
  if (program->stmts == NULL) {
    printf("program->stmts failed memory\n");
    exit(1);
  }
  return program;
}

void program_append(Program *program, Stmt *stmt) {
  if (program->len >= program->cap) {
    program->cap *= 2;
    program->stmts =
        (Stmt **)realloc(program->stmts, sizeof(Stmt *) * program->cap);
    if (program->stmts == NULL) {
      printf("program->stmts failed memory\n");
      exit(1);
    }
  }

  program->stmts[program->len] = stmt;
  program->len++;
}

// STMT start
//
//
//
void stmt_print(Stmt *stmt) {
  char *kind_str = stmt_kind_to_str(stmt->kind);

  switch (stmt->kind) {
  case STMT_EXP:
    printf("kind: %s, ", kind_str);
    exp_print(stmt->data.exp);
    break;
  case STMT_LET:
    printf("kind: %s, let %s = %s;\n", kind_str, stmt->data.let.name,
           exp_kind_to_str(stmt->data.let.value->kind));
    exp_print(stmt->data.let.value);
    break;
  case STMT_CONST:
    printf("kind: %s, const %s = %s;\n", kind_str, stmt->data.constant.name,
           exp_kind_to_str(stmt->data.constant.value->kind));
    exp_print(stmt->data.constant.value);
    break;
  case STMT_RETURN:
    printf("return ");
    exp_print(stmt->data.ret.value);
    break;
  case STMT_BLOCK:
    printf("{\n");
    for (size_t i = 0; i < stmt->data.block.len; i++) {
      stmt_print(stmt->data.block.stmts[i]);
    }
    printf("}\n");
    break;
  case STMT_IF_ELSE:
    printf("if else\n");
    break;
  case STMT_REASSIGN:
    printf("%s = ", stmt->data.reassign.name);
    exp_print(stmt->data.reassign.value);
    break;
  }
}

char *stmt_kind_to_str(StmtKind kind) {
  switch (kind) {
  case STMT_LET:
    return "LET";
  case STMT_CONST:
    return "CONST";
  case STMT_REASSIGN:
    return "REASSIGN";
  case STMT_RETURN:
    return "RETURN";
  case STMT_EXP:
    return "EXP";
  case STMT_BLOCK:
    return "BLOCK";
  case STMT_IF_ELSE:
    return "IFELSE";
  }
  printf("stmt_kind_to_str unknown stmt kind %d\n", kind);
  exit(1);
}

Stmt *stmt_new() {
  Stmt *stmt = (Stmt *)malloc(sizeof(Stmt));
  if (stmt == NULL) {
    printf("stmt memory error\n");
    exit(1);
  }
  return stmt;
}

Stmt *stmt_let_new(char *name, Exp *value) {
  Stmt *stmt = stmt_new();
  stmt->kind = STMT_LET;
  stmt->data.let.name = name;
  stmt->data.let.value = value;
  return stmt;
}

Stmt *stmt_const_new(char *name, Exp *value) {
  Stmt *stmt = stmt_new();
  stmt->kind = STMT_CONST;
  stmt->data.constant.name = name;
  stmt->data.constant.value = value;
  return stmt;
}

Stmt *stmt_ret_new(Exp *value) {
  Stmt *stmt = stmt_new();
  stmt->kind = STMT_RETURN;
  stmt->data.ret.value = value;
  return stmt;
}

Stmt *stmt_exp_new(Exp *exp) {
  Stmt *stmt = stmt_new();
  stmt->kind = STMT_EXP;
  stmt->data.exp = exp;
  return stmt;
}

Stmt *stmt_block_new() {
  Stmt *block = stmt_new();
  if (block == NULL) {
    printf("stmt block failed memory\n");
    exit(1);
  }
  block->kind = STMT_BLOCK;
  block->data.block.len = 0;
  block->data.block.cap = 10;
  block->data.block.stmts =
      (Stmt **)malloc(sizeof(Stmt *) * block->data.block.cap);
  if (block->data.block.stmts == NULL) {
    printf("block->stmts failed memory\n");
    exit(1);
  }
  return block;
}

Stmt *stmt_if_else_new(Exp *condition, Stmt *consequence, Stmt *alternative) {
  Stmt *if_else = stmt_new();
  if_else->kind = STMT_IF_ELSE;
  if_else->data.if_else.condition = condition;
  if_else->data.if_else.consequence = consequence;
  if_else->data.if_else.alternative = alternative;

  return if_else;
}

Stmt *stmt_reassign_new(char *name, Exp *value) {
  Stmt *reassign = stmt_new();
  reassign->kind = STMT_REASSIGN;
  reassign->data.reassign.name = name;
  reassign->data.reassign.value = value;
  return reassign;
}

void stmt_block_append(Stmt *block, Stmt *inner_stmt) {
  if (block->data.block.len >= block->data.block.cap) {
    block->data.block.cap *= 2;
    block->data.block.stmts = (Stmt **)realloc(
        block->data.block.stmts, sizeof(Stmt *) * block->data.block.cap);
    if (block->data.block.stmts == NULL) {
      printf("program->stmts failed memory\n");
      exit(1);
    }
  }

  block->data.block.stmts[block->data.block.len] = inner_stmt;
  block->data.block.len++;
}
//
//
//
// STMT end

// EXP start
//
//
//
void exp_print(Exp *exp) {
  char *kind_str = exp_kind_to_str(exp->kind);

  switch (exp->kind) {
  case EXP_NUMBER:
    printf("kind: %s, number: |%f|\n", kind_str, exp->data.number);
    break;
  case EXP_STRING:
    printf("kind: %s, string: |%s|\n", kind_str, exp->data.string);
    break;
  case EXP_ARRAY:
    printf("[\n");
    exp_print(exp->data.array.list);
    printf("]\n");
    break;
  case EXP_IDENT:
    printf("kind: %s, ident: |%s|\n", kind_str, exp->data.string);
    break;
  case EXP_LIST:
    for (size_t i = 0; i < exp->data.list.len; i++) {
      exp_print(exp->data.list.elements[i]);
      printf(",");
    }
    break;
  case EXP_FN:
    printf("\nfn %s(\n", exp->data.fn.name);
    exp_print(exp->data.fn.params);
    printf(")");
    stmt_print(exp->data.fn.body);
    break;
  case EXP_CALL:
    printf("(");
    printf(")");
    break;
  case EXP_BOOL:
    printf("%s", exp->data.boolean ? "TRUE" : "FALSE");
    break;
  /* case EXP_LOOP: */
  /*   printf("for \n"); */
  /*   break; */
  case EXP_INFIX:
    printf("infix\n");
    break;
  case EXP_NIL:
    printf("NIL");
    break;
  }
}

char *exp_kind_to_str(ExpKind kind) {
  switch (kind) {
  case EXP_NUMBER:
    return "NUMBER";
  case EXP_STRING:
    return "STRING";
  case EXP_LIST:
    return "LIST";
  case EXP_ARRAY:
    return "ARRAY";
  case EXP_IDENT:
    return "IDENT";
  case EXP_FN:
    return "FN";
  case EXP_CALL:
    return "CALL";
  case EXP_BOOL:
    return "BOOL";
  case EXP_NIL:
    return "NIL";
  case EXP_INFIX:
    return "INFIX";
    /* case EXP_LOOP: */
    /*   return "LOOP"; */
  }
  printf("exp_kind_to_str unknown exp kind %d\n", kind);
  exit(1);
}

Exp *exp_new() {
  Exp *exp = (Exp *)malloc(sizeof(Exp));
  return exp;
}

Exp *exp_ident_new(char *ident) {
  Exp *exp = exp_new();
  exp->kind = EXP_IDENT;
  exp->data.string = ident;
  return exp;
}

Exp *exp_string_new(char *string) {
  Exp *exp = exp_new();
  exp->kind = EXP_STRING;
  exp->data.string = string;
  return exp;
}

Exp *exp_number_new(double value) {
  Exp *exp = exp_new();
  exp->kind = EXP_NUMBER;
  exp->data.number = value;
  return exp;
}

Exp *exp_fn_new(char *name, Exp *params, Stmt *body) {
  Exp *exp = exp_new();
  exp->kind = EXP_FN;
  exp->data.fn.name = name;
  exp->data.fn.params = params;
  exp->data.fn.body = body;
  return exp;
}

Exp *exp_call_new(char *name, Exp *args) {
  Exp *exp = exp_new();
  exp->kind = EXP_CALL;
  exp->data.call.name = name;
  exp->data.call.args = args;
  return exp;
}

Exp *exp_bool_new(bool boolean) {
  Exp *exp = exp_new();
  exp->kind = EXP_BOOL;
  exp->data.boolean = boolean;
  return exp;
}

Exp *exp_nil_new() {
  Exp *exp = exp_new();
  exp->kind = EXP_NIL;
  return exp;
}

Exp *exp_infix_new(TokenKind op, Exp *left, Exp *right) {
  Exp *exp = exp_new();
  exp->kind = EXP_INFIX;
  exp->data.infix.op = op;
  exp->data.infix.left = left;
  exp->data.infix.right = right;
  return exp;
}

Exp *exp_array_new(Exp *list) {
  Exp *exp = exp_new();
  exp->kind = EXP_ARRAY;
  exp->data.array.list = list;
  return exp;
}

/* Exp *exp_loop_new() { */
/* } */

Exp *exp_list_new() {
  Exp *exp = exp_new();
  exp->kind = EXP_LIST;
  exp->data.list.len = 0;
  exp->data.list.cap = 1;
  Exp **elements = (Exp **)malloc(sizeof(Exp *) * exp->data.list.cap);
  exp->data.list.elements = elements;
  return exp;
}

void exp_list_append(Exp *list, Exp *element) {
  if (list->data.list.len >= list->data.list.cap) {
    list->data.list.cap *= 2;
    list->data.list.elements = (Exp **)realloc(
        list->data.list.elements, sizeof(Exp *) * list->data.list.cap);
    if (list->data.list.elements == NULL) {
      printf("list->elements failed memory\n");
      exit(1);
    }
  }
  list->data.list.elements[list->data.list.len] = element;
  list->data.list.len++;
}
//
//
//
// EXP end
