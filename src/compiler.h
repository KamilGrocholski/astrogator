#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "node.h"
#include "object.h"

#ifndef _COMPILER_H
#define _COMPILER_H

typedef enum {
  OP_CONST,
  OP_ADD,
} OpKind;

typedef struct {
  Obj **objs;
  size_t len;
  size_t cap;
} ObjList;

typedef struct {
  char name[32];
  uint8_t operands;
  uint8_t operand_widths[8];
} Def;

typedef struct {
  uint8_t *bytes;
  uint32_t cap;
  uint32_t len;
} Inst;

typedef struct {
  Inst **insts;
  size_t len;
  size_t cap;
} InstList;

typedef struct {
  Inst *insts;
  ObjList *cnsts;
} Bytecode;

typedef struct {
  InstList *insts;
  ObjList *cnsts;
} Compiler;

void compiler_init(Compiler *compiler);
void compiler_compile(Compiler *compiler, Program *program);

Def opkind_def_lookup(OpKind kind);

ObjList *objlist_new();
size_t objlist_append(ObjList *list, Obj *obj);

InstList *instlist_new();
void instlist_append(InstList *list, Inst *inst);
Inst *inst_new(OpKind opkind, ...);
Inst *inst_new_from_va_list(OpKind opkind, va_list operands);

#endif // !_COMPILER_H
