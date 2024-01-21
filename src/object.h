#include <stdbool.h>
#include <stddef.h>

#ifndef _OBJECT_H
#define _OBJECT_H

typedef enum {
  OBJ_NUMBER,
  OBJ_STRING,
} ObjKind;

typedef struct {
  ObjKind kind;
  union {
    double number;

    struct {
      char *value;
      size_t len;
    } string;

    struct {
      void *value;
      bool marked;
    } heap;

  } data;
} Obj;

Obj *obj_new();
Obj *obj_string_new(char *value, size_t len);
Obj *obj_number_new(double value);

#endif //_OBJECT_H
