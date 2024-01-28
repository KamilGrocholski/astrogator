#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "st.h"

// TODO just for now
#define INITIAL_CAP 4000

St *st_new(St *env) {
  St *st = malloc(sizeof(St));
  if (st == NULL) {
    exit(1);
  }
  st->len = 0;
  st->cap = INITIAL_CAP;
  st->env = env;

  st->table = malloc(sizeof(StEntry *) * INITIAL_CAP);
  if (st->table == NULL) {
    free(st);
    exit(1);
  }

  for (uint32_t i = 0; i < INITIAL_CAP; i++) {
    st->table[i] = NULL;
  }

  return st;
}

void st_entry_free(StEntry *entry) { free(entry); }

void st_free(St *st) {
  for (uint32_t i = 0; i < st->len; i++) {
    st_entry_free(st->table[i]);
  }
  free(st->table);
  free(st);
}

StEntry *st_lookup(St *st, char *key) {
  for (uint32_t i = 0; i < st->len; i++) {
    if (strcmp(st->table[i]->key, key) == 0) {
      return st->table[i];
    }
  }
  return NULL;
}

StEntry *st_insert(St *st, char *key) {
  StEntry *entry = malloc(sizeof(StEntry));
  if (entry == NULL) {
    printf("entry mem alloc err\n");
    exit(1);
  }

  entry->entry_scope =
      (st->env == NULL) ? ST_ENTRY_SCOPE_GLOBAL : ST_ENTRY_SCOPE_LOCAL;
  entry->key = key;
  entry->value = st->len;
  st->table[st->len] = entry;
  st->len++;

  printf("ENTRY: value: %d, scope: %d, key: %s\n", entry->value,
         entry->entry_scope, entry->key);

  return entry;
}

void st_set_value(St *st, char *key, uint32_t new_value) {
  for (uint32_t i = 0; i < st->len; i++) {
    if (strcmp(st->table[i]->key, key) == 0) {
      st->table[i]->value = new_value;
    }
  }
}
