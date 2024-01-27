#include <stdarg.h>
#include <stdio.h>

#include "../src/code.h"
#include "../src/vm.h"

uint32_t encode_instruction_test(OpCode opcode, ...) {
  va_list args;
  va_start(args, opcode);

  uint32_t result = encode_instruction(opcode, args);

  va_end(args);
  return result;
}

int main() {
  InstructionList *instructionlist = instructionlist_new();
  ObjList *constants = objlist_new();

  objlist_append(constants, &(Obj){.kind = OBJ_NUMBER, .data.number = 10});
  objlist_append(constants, &(Obj){.kind = OBJ_NUMBER, .data.number = 20});
  objlist_append(constants, &(Obj){.kind = OBJ_NUMBER, .data.number = 5});

  instructionlist_append(instructionlist, encode_instruction_test(OP_CONST, 0));
  instructionlist_append(instructionlist, encode_instruction_test(OP_CONST, 1));
  instructionlist_append(instructionlist, encode_instruction_test(OP_ADD));
  instructionlist_append(instructionlist, encode_instruction_test(OP_CONST, 2));
  instructionlist_append(instructionlist, encode_instruction_test(OP_DIVIDE));

  Vm *vm = vm_new(instructionlist, constants);
  vm_run(vm);
  vm_free(vm);

  return 0;
}
