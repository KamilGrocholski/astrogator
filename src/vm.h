#include <stddef.h>

#include "compiler.h"
#include "object.h"

#ifndef _VM_H
#define _VM_H

typedef struct {
  ObjList *stack;
  size_t stack_ptr;
  ObjList *cnsts;
} Vm;

void vm_init(Vm *vm, Bytecode *bytecode);
void vm_start(Vm *vm);

#endif // !_VM_H
