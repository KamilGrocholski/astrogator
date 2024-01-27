#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "code.h"

const uint8_t op_argsc[] = {
    [OP_CONST] = 1,        [OP_ADD] = 0,
    [OP_EQUAL] = 0,        [OP_NOT_EQUAL] = 0,
    [OP_GREATER_THAN] = 0, [OP_EQUAL_OR_GREATER_THAN] = 0,
    [OP_LESS_THAN] = 0,    [OP_EQUAL_OR_LESS_THAN] = 0,
    [OP_AND] = 0,          [OP_OR] = 0,
    [OP_TRUE] = 0,         [OP_FALSE] = 0,
    [OP_NULL] = 0,
};

const uint8_t op_argsw[] = {
    [OP_CONST] = 2,        [OP_ADD] = 0,
    [OP_EQUAL] = 0,        [OP_NOT_EQUAL] = 0,
    [OP_GREATER_THAN] = 0, [OP_EQUAL_OR_GREATER_THAN] = 0,
    [OP_LESS_THAN] = 0,    [OP_EQUAL_OR_LESS_THAN] = 0,
    [OP_AND] = 0,          [OP_OR] = 0,
    [OP_TRUE] = 0,         [OP_FALSE] = 0,
    [OP_NULL] = 0,
};

const char *op_str[] = {
    [OP_CONST] = "OP_CONST",
    [OP_ADD] = "OP_ADD",
    [OP_SUBTRACT] = "OP_SUBTRACT",
    [OP_MULTIPLY] = "OP_MULTIPLY",
    [OP_DIVIDE] = "OP_DIVIDE",
    [OP_EQUAL] = "OP_EQUAL",
    [OP_NOT_EQUAL] = "OP_NOT_EQUAL",
    [OP_GREATER_THAN] = "OP_GREATER_THAN",
    [OP_EQUAL_OR_GREATER_THAN] = "OP_EQUAL_OR_GREATER_THAN",
    [OP_LESS_THAN] = "OP_LESS_THAN",
    [OP_EQUAL_OR_LESS_THAN] = "OP_EQUAL_OR_LESS_THAN",
    [OP_AND] = "OP_AND",
    [OP_OR] = "OP_OR",
    [OP_TRUE] = "OP_TRUE",
    [OP_FALSE] = "OP_FALSE",
    [OP_NULL] = "OP_NULL",
};

const char *op_to_str(OpCode opcode) { return op_str[opcode]; }

OpCode read_opcode(uint32_t instruction) { return (OpCode)(instruction >> 24); }

uint32_t encode_instruction(OpCode opcode, va_list args) {
  uint32_t instruction = (opcode << 24);

  uint8_t argsc = op_argsc[opcode];
  for (uint8_t i = 0; i < argsc; i++) {
    uint32_t arg = va_arg(args, uint32_t);
    instruction |= (arg & 0xFFFFFF) << (i * 8);
  }

  return instruction;
}

uint8_t read_8bits(uint32_t instruction, uint32_t offset) {
  /* #ifdef DEBUG */
  /*   printf("read_8bits: (instruction: %u, offset: %u)\n", instruction,
   * offset); */
  /* #endif /1* ifdef DEBUG *1/ */
  return (instruction >> (offset * 8)) & 0xFF;
}

uint16_t read_16bits(uint32_t instruction, uint32_t offset) {
  /* #ifdef DEBUG */
  /*   printf("read_16bits: (instruction: %u, offset: %u)\n", instruction,
   * offset); */
  /* #endif /1* ifdef DEBUG *1/ */
  return (instruction >> (offset * 8)) & 0xFFFF;
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
  if (instructionlist->list == NULL) {
    printf("instructionlist->list mem alloc err\n");
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
