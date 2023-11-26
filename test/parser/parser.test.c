#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "../../src/compiler/parser.h"
#include "parser.test.h"

void test_parser_stmt_let_int() {
  int expected_val_type = VALUE_TYPE_INT;
  char expected_ident[] = "x";
  int expected_value = 99;

  char *valid_input = "let x: int = 99;";

  Lexer *lexer = lexer_new(valid_input);
  Parser *parser = parser_new(lexer);

  Program *program = parser_parse_program(parser);

  Stmt *let_stmt = program->head->stmt;

  assert(let_stmt->kind == STMT_KIND_LET);
  assert(strcmp(let_stmt->u.let.name.u.ident, expected_ident) == 0);
  assert(let_stmt->u.let.value_type == expected_val_type);
  assert(let_stmt->u.let.value.u.int_value == expected_value);
}

void test_parser_stmt_let_string() {
  int expected_val_type = VALUE_TYPE_STRING;
  char expected_ident[] = "x";
  char expected_value[] = "oke";

  char *valid_input = "let x: string = \"oke\";";

  Lexer *lexer = lexer_new(valid_input);
  Parser *parser = parser_new(lexer);

  Program *program = parser_parse_program(parser);

  Stmt *let_stmt = program->head->stmt;

  assert(let_stmt->kind == STMT_KIND_LET);
  assert(strcmp(let_stmt->u.let.name.u.ident, expected_ident) == 0);
  assert(let_stmt->u.let.value_type == expected_val_type);
  assert(strcmp(let_stmt->u.let.value.u.string_value, expected_value) == 0);
}

void test_parser_stmt_let_bool() {
  int expected_val_type = VALUE_TYPE_BOOL;
  char expected_ident[] = "x";
  uint8_t expected_value = FALSE;

  char *valid_input = "let x: bool = false;";

  Lexer *lexer = lexer_new(valid_input);
  Parser *parser = parser_new(lexer);

  Program *program = parser_parse_program(parser);

  Stmt *let_stmt = program->head->stmt;

  assert(let_stmt->kind == STMT_KIND_LET);
  assert(strcmp(let_stmt->u.let.name.u.ident, expected_ident) == 0);
  assert(let_stmt->u.let.value_type == expected_val_type);
  assert(let_stmt->u.let.value.u.bool_value == expected_value);
}
