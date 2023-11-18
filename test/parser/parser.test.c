#include <stdio.h>
#include <string.h>

#include "../../src/compiler/parser/parser.h"

#include "parser.test.h"
#include <assert.h>

void test_parser_stmt_let() {
  // TODO

  size_t expected_stmts_len = 1;
  int expected_val_type = VAL_TYPE_INT;
  char expected_ident[] = "x";
  int expected_value = 99;

  char *valid_input = "let x: int = 99;";

  Lexer *lexer = lexer_new(valid_input);
  Parser *parser = parser_new(lexer);

  Program *program = parser_parse_program(parser);

  /* printf("%zu\n", program->stmts_idx); */

  assert(expected_stmts_len == program->stmts_idx);

  Stmt stmt = program->stmts[0];
  /* printf("%s\n", &stmt.sb.let_decl_assign.ident); */
  /* printf("%d\n", stmt.sb.let_decl_assign.type); */

  assert(strncmp(&stmt.sb.let_decl_assign.ident, expected_ident, 1) == 0);
  assert(stmt.sb.let_decl_assign.type == expected_val_type);
  assert(stmt.sb.let_decl_assign.value->eb.integer_lit == expected_value);
}
