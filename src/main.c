#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "lexer.h"
#include "parser.h"

int main() {
  /* char *file_name = "index.ag"; */
  char *file_name = "tocomp.ag";
  size_t len = strlen(file_name);

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Err opening file");
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *input = (char *)malloc(file_size + 1);
  if (input == NULL) {
    perror("Mem alloc err");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  fread(input, 1, file_size, file);
  input[file_size] = '\0';

  fclose(file);

  Lexer lexer;
  lexer_init(&lexer, input, file_size);
  Parser *parser = parser_new(&lexer);
  Program *program = program_new();

  while (parser->curr_token.kind != TOKEN_EOF &&
         parser->curr_token.kind != TOKEN_ILLEGAL) {
    Stmt *stmt = parser_parse_stmt(parser);
    if (stmt != NULL) {
      /* stmt_print(stmt); */
      program_append(program, stmt);
    } else {
      printf("null stmt in program\n");
      exit(EXIT_FAILURE);
    }
  }

  Compiler compiler;
  compiler_init(&compiler);
  compiler_compile(&compiler, program);

  free(input);

  return 0;
}
