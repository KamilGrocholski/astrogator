#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "code.h"

const uint8_t op_argsc[] = {
    [OP_CONST] = 1,
    [OP_ADD] = 0,
};

const uint8_t op_argsw[] = {
    [OP_CONST] = 2,
    [OP_ADD] = 0,
};

const char *op_str[] = {
    [OP_CONST] = "OP_CONST",
    [OP_ADD] = "OP_ADD",
};

const char *op_to_str(OpCode opcode) { return op_str[opcode]; }

uint32_t encode_instruction(OpCode opcode, ...) {
  va_list args;
  va_start(args, opcode);

  uint32_t instruction = (opcode << 24);

  uint8_t argsc = op_argsc[opcode];
  for (uint8_t i = 0; i < argsc; i++) {
    uint32_t arg = va_arg(args, uint32_t);
    instruction |= (arg & 0xFFFFFF) << ((i) * 8);
  }

  va_end(args);

  return instruction;
}

OpCode read_opcode(uint32_t instruction) { return (OpCode)(instruction >> 24); }

uint8_t read_8bits(uint32_t instruction, uint32_t offset) {
  return (instruction >> ((offset - 1) * 8)) & 0xFF;
}

uint16_t read_16bits(uint32_t instruction, uint32_t offset) {
  return (instruction >> ((offset - 1) * 8)) & 0xFFFF;
}

InstructionList *instructionlist_new() {
  InstructionList *instructionlist = malloc(sizeof(InstructionList));
  if (instructionlist == NULL) {
    printf("instructionlist mem alloc err\n");
    exit(1);
  }
  instructionlist->len = 0;
  instructionlist->cap = 1;
  instructionlist->list = malloc(sizeof(uint32_t) * instructionlist->cap);
  if (instructionlist == NULL) {
    printf("instructionlist-> mem alloc err\n");
    exit(1);
  }

  return instructionlist;
}

void instructionlist_append(InstructionList *instructionlist,
                            uint32_t instruction) {
  if (instructionlist->len >= instructionlist->cap) {
    instructionlist->cap *= 2;
    instructionlist->list =
        realloc(instructionlist->list, sizeof(uint32_t) * instructionlist->cap);
    if (instructionlist->list == NULL) {
      printf("instructionlist>list mem realloc err\n");
      exit(1);
    }
  }
  instructionlist->list[instructionlist->len] = instruction;
  instructionlist->len++;
}

/* int main() { */
/*   uint32_t inst = encode_instruction(OP_CONST, 10, 0, 1); */

/*   OpCode opcode = read_opcode(inst); */
/*   printf("opcode %d\n", opcode); */

/*   switch (opcode) { */
/*   case OP_CONST: { */
/*     uint8_t arg1 = read_8bits(inst, 1); */
/*     uint16_t arg2 = read_16bits(inst, 2); */

/*     printf("Arg1: %u\n", arg1); */
/*     printf("Arg2: %u\n", arg2); */
/*   } break; */
/*   case OP_ADD: { */
/*     // Handle OP_ADD case if needed */
/*   } break; */
/*   default: */
/*     printf("Unknown opcode %d\n", opcode); */
/*     exit(1); */
/*     break; */
/*   } */

/*   return 0; */
/* } */
