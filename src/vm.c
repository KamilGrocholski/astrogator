#include "vm.h"
#include <stdint.h>

#define DISPATCH() goto *dispatch_table[vm->cnsts->objs[idx]->kind];

void vm_stack_push(Vm *vm, Obj *obj);

void vm_init(Vm *vm, Bytecode *bytecode) {
  *vm = (Vm){
      .cnsts = bytecode->cnsts,
      .stack = objlist_new(),
      .stack_ptr = 0,
  };
}

void vm_start(Vm *vm) {
  const void *dispatch_table[] = {
      &&GOTO_OP_CONST,
      &&GOTO_OP_ADD,
  };

GOTO_OP_CONST: {
  uint16_t idx = read_uint16(vm->stack[vm->stack_ptr]);
  vm_stack_push(vm, vm->cnsts->objs[idx]);
  DISPATCH();
}

GOTO_OP_ADD: {}
}

void vm_stack_push(Vm *vm, Obj *obj) {}
