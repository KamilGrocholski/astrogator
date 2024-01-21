#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "object.h"

void compile_stmt(Compiler *compiler, Stmt *stmt);

void compile_exp(Compiler *compiler, Exp *exp);
void compile_exp_infix(Compiler *compiler, Exp *exp_infix);

size_t compiler_inst_push(Compiler *compiler, Inst *inst);

void compiler_init(Compiler *compiler) {
  *compiler = (Compiler){
      .cnsts = objlist_new(),
      .insts = instlist_new(),
  };
}

void compiler_compile(Compiler *compiler, Program *program) {
  for (size_t i = 0; i < program->len; i++) {
    compile_stmt(compiler, program->stmts[i]);
  }

  for (size_t i = 0; i < compiler->insts->len; i++) {
    for (size_t j = 0; j < compiler->insts->insts[j]->len; j++) {
      printf("%d|", compiler->insts->insts[i]->bytes[j]);
    }
    printf("\n");
  }

  for (size_t i = 0; i < compiler->cnsts->len; i++) {
    printf("%d\n", compiler->cnsts->objs[i]->kind);
  }
}

void compile_stmt(Compiler *compiler, Stmt *stmt) {
  switch (stmt->kind) {
  case STMT_EXP: {
    compile_exp(compiler, stmt->data.exp);
  } break;
  }
}

void compile_exp(Compiler *compiler, Exp *exp) {
  switch (exp->kind) {
  case EXP_NUMBER: {
    Obj *obj = obj_number_new(exp->data.number);
    objlist_append(compiler->cnsts, obj);
    compiler_inst_push(compiler, inst_new(OP_CONST, obj));
  } break;
  }
}

void compile_exp_infix(Compiler *compiler, Exp *exp_infix) {
  compile_exp(compiler, exp_infix->data.infix.left);
  compile_exp(compiler, exp_infix->data.infix.right);

  switch (exp_infix->data.infix.op) {
  case TOKEN_PLUS: {
    compiler_inst_push(compiler, inst_new(OP_ADD));
  } break;
  }
}

static Def defs[] = {
    {"OP_CONST", 1, {2}},
    {"OP_ADD", 0, {}},
};

Def opkind_def_lookup(OpKind kind) { return defs[kind]; }

size_t compiler_inst_push(Compiler *compiler, Inst *inst) {
  size_t pos = compiler->insts->len;
  instlist_append(compiler->insts, inst);
  return pos;
}

InstList *instlist_new() {
  InstList *list = malloc(sizeof(InstList));
  list->cap = 1;
  list->len = 0;
  list->insts = malloc(sizeof(Inst) * list->cap);
  return list;
}

void instlist_append(InstList *list, Inst *inst) {
  if (list->len >= list->cap) {
    list->cap *= 2;
    list->insts = (Inst **)realloc(list->insts, sizeof(Inst *) * list->cap);
    if (list->insts == NULL) {
      printf("instlist alloc err mem\n");
      exit(1);
    }
  }
  list->insts[list->len++] = inst;
}

Inst *inst_new(OpKind kind, ...) {
  va_list args;
  va_start(args, kind);
  Inst *inst = inst_new_from_va_list(kind, args);
  va_end(args);
  return inst;
}

Inst *inst_new_from_va_list(OpKind kind, va_list operands) {
  Def def = opkind_def_lookup(kind);
  Inst *inst = (Inst *)malloc(sizeof(Inst));
  inst->bytes = (uint8_t *)malloc(sizeof(uint8_t) * (def.operands + 1) * 3);
  inst->bytes[0] = kind;
  inst->len = 1;
  inst->cap = def.operands + 1;

  for (uint8_t op_idx = 0; op_idx < def.operands; op_idx++) {
    int64_t operand = va_arg(operands, int);
    for (int8_t byte_idx = def.operand_widths[op_idx] - 1; byte_idx >= 0;
         byte_idx--) {
      inst->bytes[inst->len] = (uint8_t)(operand >> (byte_idx * 8));
      inst->len++;
    }
  }

  return inst;
}

ObjList *objlist_new() {
  ObjList *c = (ObjList *)malloc(sizeof(ObjList));
  c->cap = 2;
  c->len = 0;
  c->objs = (Obj **)malloc(sizeof(Obj *) * c->cap);
  return c;
}

size_t objlist_append(ObjList *objlist, Obj *obj) {
  if (objlist->len >= objlist->cap) {
    objlist->cap *= 2;
    objlist->objs =
        (Obj **)realloc(objlist->objs, sizeof(Obj *) * objlist->cap);
    if (objlist->objs == NULL) {
      printf("constants->ojbs alloc err mem\n");
      exit(1);
    }
  }
  objlist->objs[objlist->len++] = obj;
  return objlist->len - 1;
}
