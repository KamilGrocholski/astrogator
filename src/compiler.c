#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

void compile_stmt(Compiler *compiler, Stmt *stmt);
void compile_exp(Compiler *compiler, Exp *exp);

uint32_t emit(Compiler *compiler, OpCode opcode, ...);

Compiler *compiler_new() {
  Compiler *compiler = malloc(sizeof(Compiler));
  if (compiler == NULL) {
    printf("compiler mem alloc err\n");
    exit(1);
  }
  compiler->constants = objlist_new();
  compiler->instructions = instructionlist_new();

  return compiler;
}

void compiler_free(Compiler *compiler) { free(compiler); }

void compiler_compile(Compiler *compiler, Program *program) {
  for (size_t i = 0; i < program->len; i++) {
#ifdef DEBUG
    stmt_print(program->stmts[i]);
#endif /* ifdef DEBUG */
    compile_stmt(compiler, program->stmts[i]);
  }
}

void compile_stmt(Compiler *compiler, Stmt *stmt) {
  switch (stmt->kind) {
  case STMT_EXP:
    compile_exp(compiler, stmt->data.exp);
    break;
  }
}

void compile_exp(Compiler *compiler, Exp *exp) {
#ifdef DEBUG
  exp_print(exp);
#endif /* ifdef DEBUG */
  switch (exp->kind) {
  case EXP_INFIX: {
    compile_exp(compiler, exp->data.infix.left);
    compile_exp(compiler, exp->data.infix.right);
    switch (exp->data.infix.op) {
    case TOKEN_PLUS:
      emit(compiler, OP_ADD);
      break;
    }
  } break;
  case EXP_NUMBER: {
    Obj *obj = obj_number_new(exp->data.number);
    objlist_append(compiler->constants, obj);
    emit(compiler, OP_CONST, compiler->constants->len - 1);
  } break;
  }
}

uint32_t emit(Compiler *compiler, OpCode opcode, ...) {
  va_list args;
  va_start(args, opcode);
  uint32_t instruction = encode_instruction(opcode, args);
  va_end(args);
  instructionlist_append(compiler->instructions, instruction);
  return compiler->instructions->len;
}
