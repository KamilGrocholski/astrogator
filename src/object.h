#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

typedef struct {
  Obj *list;
  uint32_t len;
  uint32_t cap;
} ObjList;

ObjList *objlist_new();
void objlist_insert(ObjList *objlist, Obj *obj, uint32_t index);
void objlist_append(ObjList *objlist, Obj *obj);
void obj_print(Obj *obj);
Obj *obj_new();
Obj *obj_string_new(char *value, size_t len);
Obj *obj_number_new(double value);

#endif //_OBJECT_H
