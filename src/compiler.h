#include <stdint.h>

#include "code.h"
#include "node.h"
#include "object.h"
#include "st.h"

#ifndef _COMPILER_H
#define _COMPILER_H

// TODO add scopes
typedef struct {
  St *symbol_table;
  InstructionList *instructions;
  ObjList *constants;
} Compiler;

Compiler *compiler_new();
void compiler_free(Compiler *compiler);
void compiler_compile(Compiler *compiler, Program *program);

#endif // !_COMPILER_H
