#include <stdint.h>

#include "code.h"
#include "object.h"

#ifndef _VM_H
#define _VM_H

typedef struct {
  uint8_t ip;
  uint32_t sp;
  ObjList *stack;
  ObjList *constants;
  InstructionList *instructions;
} Vm;

Vm *vm_new(InstructionList *instructionlist, ObjList *constants);
void vm_run(Vm *vm);
void vm_free(Vm *vm);

#endif // !_VM_H
