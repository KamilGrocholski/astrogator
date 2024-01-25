#include "../src/vm.h"

int main() {
  Obj stack[] = {
      (Obj){.kind = OBJ_NUMBER, .data.number = 10.0},
      (Obj){.kind = OBJ_NUMBER, .data.number = 20.0},

      (Obj){.kind = OBJ_BOOLEAN, .data.boolean = true},
      (Obj){.kind = OBJ_BOOLEAN, .data.boolean = false},
  };

  Instruction instructions[] = {
      *instruction_new(OP_CONST, 0), *instruction_new(OP_CONST, 1),
      *instruction_new(OP_ADD, 0),   *instruction_new(OP_PRINT, 0),

      *instruction_new(OP_CONST, 2), *instruction_new(OP_CONST, 3),
      *instruction_new(OP_EQUAL, 0), *instruction_new(OP_PRINT, 0),
  };

  Vm vm;
  vm_init(&vm, instructions, sizeof(instructions) / sizeof(instructions[0]),
          stack, 30);
  vm_run(&vm);
  vm_free(&vm);

  return 0;
}
