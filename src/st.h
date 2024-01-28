#include <stdint.h>

#ifndef _ST_H
#define _ST_H

typedef enum {
  ST_ENTRY_SCOPE_GLOBAL,
  ST_ENTRY_SCOPE_LOCAL,
} StEntryScope;

typedef struct StEntry {
  char *key;
  uint32_t value;
  StEntryScope entry_scope;
} StEntry;

// TODO later change from array to hashmap
typedef struct St {
  StEntry **table;
  uint32_t cap;
  uint32_t len;
  struct St *env;
} St;

St *st_new(St *env);
void st_entry_free(StEntry *entry);
void st_free(St *st);
StEntry *st_lookup(St *st, char *key);
StEntry *st_insert(St *st, char *key);
void st_set_value(St *st, char *key, uint32_t new_value);

#endif // !_ST_H
