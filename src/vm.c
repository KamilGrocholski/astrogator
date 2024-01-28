#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

// TODO create a separate str utils
bool str_cmp(char *v1, size_t l1, char *v2, size_t l2);

void vm_stack_push(Vm *vm, Obj *obj);
Obj *vm_stack_pop(Vm *vm);
Obj *vm_stack_curr(Vm *vm);

void vm_exec_binary(Vm *vm, OpCode opcode);
void vm_exec_binary_number(Vm *vm, OpCode opcode, Obj *left, Obj *right);

void vm_exec_compare(Vm *vm, OpCode opcode);
void vm_exec_compare_number(Vm *vm, OpCode opcode, Obj *left, Obj *right);
void vm_exec_compare_boolean(Vm *vm, OpCode opcode, Obj *left, Obj *right);
void vm_exec_compare_string(Vm *vm, OpCode opcode, Obj *left, Obj *right);

Vm *vm_new(InstructionList *instructionlist, ObjList *constants) {
  Vm *vm = malloc(sizeof(Vm));
  vm->ip = 0;
  vm->sp = 0;
  vm->constants = constants;
  vm->globals = objlist_new();
  vm->stack = objlist_new();
  vm->instructions = instructionlist;

  return vm;
}

void vm_run(Vm *vm) {
#ifdef DEBUG
  printf("START :: Initial globals state --\n");
  for (int32_t i = vm->globals->len - 1; i >= 0; i--) {
    printf("global[%d]: ", i);
    obj_print(&vm->globals->list[i]);
  }
  printf("END :: Initial globals state --\n");

  printf("START :: Instructions --\n");
  for (int32_t i = vm->instructions->len - 1; i >= 0; i--) {
    printf("instruction[%d]: %08X -> ", i, vm->instructions->list[i]);
    uint8_t opcode = read_opcode(vm->instructions->list[i]);
    printf("opcode: %s\n", op_to_str(opcode));
  }
  printf("END :: Instructions --\n");

  printf("START :: Constants --\n");
  for (int32_t i = vm->constants->len - 1; i >= 0; i--) {
    printf("constant[%d]: ", i);
    obj_print(&vm->constants->list[i]);
  }
  printf("END :: Constants --\n");

  printf("START :: Initial stack state --\n");
  for (int32_t i = vm->sp - 1; i >= 0; i--) {
    printf("stack[%d]: ", i);
    obj_print(&vm->stack->list[i]);
  }
  printf("END :: Initial stack state --\n");
#endif /* ifdef DEBUG */

  while (vm->ip < vm->instructions->len) {
    uint32_t curr_instruction = vm->instructions->list[vm->ip];
    uint8_t opcode = read_opcode(curr_instruction);

    /* #ifdef DEBUG */
    /*     printf("opcode: %s\n", op_to_str(opcode)); */
    /* #endif /1* ifdef DEBUG *1/ */

    switch (opcode) {
    case OP_CONST: {
      uint16_t const_idx = read_16bits(curr_instruction, 0);
      vm_stack_push(vm, &vm->constants->list[const_idx]);
    } break;
    case OP_SET_GLOBAL: {
      uint16_t idx = read_16bits(curr_instruction, 0);
      Obj *obj = vm_stack_pop(vm);
      objlist_insert(vm->globals, obj, idx);
    } break;
    case OP_GET_GLOBAL: {
      uint16_t idx = read_16bits(curr_instruction, 0);
      vm_stack_push(vm, &vm->globals->list[idx]);
    } break;
    case OP_ADD:
    case OP_SUBTRACT:
    case OP_MULTIPLY:
    case OP_DIVIDE: {
      vm_exec_binary(vm, opcode);
    } break;
    case OP_EQUAL:
    case OP_NOT_EQUAL:
    case OP_GREATER_THAN:
    case OP_EQUAL_OR_GREATER_THAN:
    case OP_LESS_THAN:
    case OP_EQUAL_OR_LESS_THAN: {
      vm_exec_compare(vm, opcode);
    } break;
    case OP_POP:
      vm_stack_pop(vm);
      break;
    default:
      printf("vm_run: invalid opcode %s\n", op_to_str(opcode));
      exit(1);
      break;
    }

    vm->ip++;
  }

#ifdef DEBUG
  printf("START :: Final globals state --\n");
  for (int32_t i = vm->globals->len - 1; i >= 0; i--) {
    printf("global[%d]: ", i);
    obj_print(&vm->globals->list[i]);
  }
  printf("END :: Final globals state --\n");

  printf("START :: Final stack state --\n");
  for (int32_t i = vm->sp - 1; i >= 0; i--) {
    printf("stack[%d]: ", i);
    obj_print(&vm->stack->list[i]);
  }
  printf("END :: Final stack state --\n");
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
  Obj *left = vm_stack_pop(vm);
  if (left->kind != right->kind) {
    printf("vm_exec_binary left kind != right kind\n");
    exit(1);
  }

  switch (left->kind) {
  case OBJ_NUMBER:
    vm_exec_binary_number(vm, opcode, left, right);
    break;
  default:
    printf("vm_exec_binary: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }
}

void vm_exec_binary_number(Vm *vm, OpCode opcode, Obj *left, Obj *right) {
  double result;
  switch (opcode) {
  case OP_ADD:
    result = left->data.number + right->data.number;
    break;
  case OP_SUBTRACT:
    result = left->data.number - right->data.number;
    break;
  case OP_MULTIPLY:
    result = left->data.number * right->data.number;
    break;
  case OP_DIVIDE:
    result = left->data.number / right->data.number;
    break;
  default:
    printf("vm_exec_binary_number: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }
  vm_stack_push(vm, obj_number_new(result));
}

void vm_exec_compare(Vm *vm, OpCode opcode) {
  Obj *right = vm_stack_pop(vm);
  Obj *left = vm_stack_pop(vm);
  if (left->kind != right->kind) {
    printf("vm_exec_compare left kind != right kind\n");
    exit(1);
  }

  switch (left->kind) {
  case OBJ_NUMBER:
    vm_exec_compare_number(vm, opcode, left, right);
    break;
  case OBJ_BOOLEAN:
    vm_exec_compare_boolean(vm, opcode, left, right);
    break;
  case OBJ_STRING:
    vm_exec_compare_string(vm, opcode, left, right);
    break;
  default:
    printf("vm_exec_compare: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }
}

void vm_exec_compare_number(Vm *vm, OpCode opcode, Obj *left, Obj *right) {
  bool result;
  switch (opcode) {
  case OP_EQUAL:
    result = left->data.number == right->data.number;
    break;
  case OP_NOT_EQUAL:
    result = left->data.number != right->data.number;
    break;
  case OP_LESS_THAN:
    result = left->data.number < right->data.number;
    break;
  case OP_EQUAL_OR_LESS_THAN:
    result = left->data.number <= right->data.number;
    break;
  case OP_GREATER_THAN:
    result = left->data.number > right->data.number;
    break;
  case OP_EQUAL_OR_GREATER_THAN:
    result = left->data.number >= right->data.number;
    break;
  default:
    printf("vm_exec_compare_number: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }

  vm_stack_push(vm, obj_boolean_new(result));
}

void vm_exec_compare_boolean(Vm *vm, OpCode opcode, Obj *left, Obj *right) {
  bool result;
  switch (opcode) {
  case OP_EQUAL:
    result = left->data.boolean == right->data.boolean;
    break;
  case OP_NOT_EQUAL:
    result = left->data.boolean != right->data.boolean;
    break;
  default:
    printf("vm_exec_compare_boolean: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }
  vm_stack_push(vm, obj_boolean_new(result));
}

// TODO later create string utils and replace the inlines
void vm_exec_compare_string(Vm *vm, OpCode opcode, Obj *left, Obj *right) {
  bool result;
  switch (opcode) {
  case OP_EQUAL: {
    result = str_cmp(left->data.string.value, left->data.string.len,
                     right->data.string.value, right->data.string.len);
  } break;
  case OP_NOT_EQUAL: {
    result = !str_cmp(left->data.string.value, left->data.string.len,
                      right->data.string.value, right->data.string.len);
  } break;
  default:
    printf("vm_exec_compare_string: invalid opcode %s\n", op_to_str(opcode));
    exit(1);
    break;
  }

  vm_stack_push(vm, obj_boolean_new(result));
}

bool str_cmp(char *v1, size_t l1, char *v2, size_t l2) {
  if (l1 != l2) {
    return false;
  }
  for (size_t i = 0; i < l1; i++) {
    if (v1[i] != v2[i]) {
      return false;
    }
  }
  return true;
}
