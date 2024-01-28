#include <stdarg.h>
#include <stdint.h>

#ifndef _CODE_H
#define _CODE_H

typedef enum {
  OP_CONST = 0,
  OP_SUBTRACT,
  OP_DIVIDE,
  OP_MULTIPLY,
  OP_ADD,
  OP_EQUAL,
  OP_NOT_EQUAL,
  OP_GREATER_THAN,
  OP_EQUAL_OR_GREATER_THAN,
  OP_LESS_THAN,
  OP_EQUAL_OR_LESS_THAN,
  OP_AND,
  OP_OR,
  OP_TRUE,
  OP_FALSE,
  OP_NULL,
  OP_RETURN_VALUE,
  OP_SET_GLOBAL,
  OP_SET_LOCAL,
  OP_GET_GLOBAL,
  OP_GET_LOCAL,
  OP_CHANGE_GLOBAL,
  OP_CHANGE_LOCAL,
  OP_POP,
} OpCode;

typedef struct {
  uint32_t *list;
  uint32_t len;
  uint32_t cap;
} InstructionList;

const char *op_to_str(OpCode opcode);
InstructionList *instructionlist_new();
void instructionlist_append(InstructionList *instructionlist,
                            uint32_t instruction);
uint32_t encode_instruction(OpCode opcode, va_list args);
OpCode read_opcode(uint32_t instruction);
uint8_t read_8bits(uint32_t instruction, uint32_t offset);
uint16_t read_16bits(uint32_t instruction, uint32_t offset);

#endif // !_CODE_H
