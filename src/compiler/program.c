#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

Program *program_new() {
  Program *program = malloc(sizeof(Program));
  program->head = NULL;
  program->tail = NULL;

  return program;
}

ProgramNode *program_node_new(Stmt *stmt) {
  ProgramNode *node = malloc(sizeof(ProgramNode));
  node->stmt = stmt;
  node->next = NULL;

  return node;
}

void program_append(Program *p, ProgramNode *node) {
  if (p->head == NULL) {
    p->head = node;
    p->tail = node;

    return;
  }

  if (p->head == p->tail) {
    p->tail = node;
    p->head->next = node;

    return;
  }

  p->tail->next = node;
  p->tail = node;
}

void program_display(Program *program) {
  ProgramNode *node = program->head;

  printf("\nProgram stmts start:\n");
  while (node != NULL) {
    printf("StmtKind: %d\n", node->stmt->kind);
    node = node->next;
  }
  printf("Program stmts end\n");
}
