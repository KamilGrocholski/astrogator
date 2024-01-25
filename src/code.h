#include <stdint.h>

#ifndef _CODE_H
#define _CODE_H

typedef enum {
  OP_CONST = 0,
  OP_ADD,
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
uint32_t encode_instruction(OpCode opcode, ...);
OpCode read_opcode(uint32_t instruction);
uint8_t read_8bits(uint32_t instruction, uint32_t offset);
uint16_t read_16bits(uint32_t instruction, uint32_t offset);

#endif // !_CODE_H
