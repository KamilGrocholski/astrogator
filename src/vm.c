#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

void vm_stack_push(Vm *vm, Obj *obj);
Obj *vm_stack_pop(Vm *vm);
Obj *vm_stack_curr(Vm *vm);

void vm_exec_binary(Vm *vm, OpCode opcode);
void vm_exec_binary_number(Vm *vm, OpCode opcode, Obj *left, Obj *right);

Vm *vm_new(InstructionList *instructionlist, ObjList *constants) {
  Vm *vm = malloc(sizeof(Vm));
  vm->ip = 0;
  vm->sp = 0;
  vm->constants = constants;
  vm->stack = objlist_new();
  vm->instructions = instructionlist;

  return vm;
}

void vm_run(Vm *vm) {
  while (vm->ip < vm->instructions->len) {
    uint32_t curr_instruction = vm->instructions->list[vm->ip];
    uint8_t opcode = read_opcode(curr_instruction);

#ifdef DEBUG
    printf("opcode: %s\n", op_to_str(opcode));
#endif /* ifdef DEBUG */

    switch (opcode) {
    case OP_CONST: {
      uint32_t const_idx = read_16bits(curr_instruction, 0);
      printf("const idx: %d, ", const_idx);
      obj_print(&vm->constants->list[const_idx]);
      vm_stack_push(vm, &vm->constants->list[const_idx]);
    } break;
    case OP_ADD:
    case OP_SUBTRACT:
    case OP_MULTIPLY:
    case OP_DIVIDE: {
      vm_exec_binary(vm, opcode);
    } break;
    }

    vm->ip++;
  }

#ifdef DEBUG
  for (uint32_t i = 0; i < vm->sp; i++) {
    printf("stack[%d]: ", i);
    obj_print(&vm->stack->list[i]);
  }
#endif /* ifdef DEBUG */
}

void vm_free(Vm *vm) { free(vm); }

void vm_stack_push(Vm *vm, Obj *obj) {
  objlist_insert(vm->stack, obj, vm->sp);
  vm->sp++;
}

Obj *vm_stack_pop(Vm *vm) {
  vm->sp--;
  return &vm->stack->list[vm->sp];
}

Obj *vm_stack_curr(Vm *vm) { return &vm->stack->list[vm->sp - 1]; }

void vm_exec_binary(Vm *vm, OpCode opcode) {
  Obj *right = vm_stack_pop(vm);
  Obj *left = vm_stack_curr(vm);
  if (left->kind != right->kind) {
    printf("vm_exec_binary left kind != right kind\n");
    exit(1);
  }

  switch (left->kind) {
  case OBJ_NUMBER:
    vm_exec_binary_number(vm, opcode, left, right);
    break;
  }
}

void vm_exec_binary_number(Vm *vm, OpCode opcode, Obj *left, Obj *right) {
  switch (opcode) {
  case OP_ADD:
    left->data.number += right->data.number;
    break;
  case OP_SUBTRACT:
    left->data.number -= right->data.number;
    break;
  case OP_MULTIPLY:
    left->data.number *= right->data.number;
    break;
  case OP_DIVIDE:
    left->data.number /= right->data.number;
    printf("CURR:");
    obj_print(vm_stack_curr(vm));
    break;
  default:
    printf("vm_exec_binary_number: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }
}
