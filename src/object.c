#include <stdio.h>
#include <stdlib.h>

#include "object.h"

#define INITIAL_OBJLIST_CAP 10

void obj_print(Obj *obj) {
  switch (obj->kind) {
  case OBJ_NUMBER:
    printf("%f", obj->data.number);
    break;
  }
  printf("\n");
}

ObjList *objlist_new() {
  ObjList *objlist = malloc(sizeof(ObjList));
  if (objlist == NULL) {
    printf("objlist mem alloc err\n");
    exit(1);
  }
  objlist->list = malloc(sizeof(Obj) * INITIAL_OBJLIST_CAP);
  if (objlist->list == NULL) {
    printf("objlist->list mem alloc err\n");
    exit(1);
  }
  objlist->len = 0;
  objlist->cap = INITIAL_OBJLIST_CAP;
  return objlist;
}

void objlist_insert(ObjList *objlist, Obj *obj, uint32_t index) {
  if (index > objlist->len) {
    printf("objlist_insert index out of bounds\n");
    exit(1);
  }

  if (objlist->len >= objlist->cap) {
    objlist->cap *= 2;
    objlist->list = realloc(objlist->list, sizeof(Obj) * objlist->cap);
    if (objlist->list == NULL) {
      printf("objlist->list mem realloc err\n");
      exit(1);
    }
  }

  for (uint32_t i = objlist->len; i > index; i--) {
    objlist->list[i] = objlist->list[i - 1];
  }

  objlist->list[index] = *obj;
  objlist->len++;
}

void objlist_append(ObjList *objlist, Obj *obj) {
  if (objlist->len >= objlist->cap) {
    objlist->cap *= 2;
    objlist->list = realloc(objlist->list, sizeof(Obj) * objlist->cap);
    if (objlist->list == NULL) {
      printf("objlist->list mem realloc err\n");
      exit(1);
    }
  }
  objlist->list[objlist->len] = *obj;
  objlist->len++;
}

Obj *obj_new() {
  Obj *obj = (Obj *)malloc(sizeof(Obj));
  if (obj == NULL) {
    printf("error alloc mem obj\n");
    exit(1);
  }
  return obj;
}

Obj *obj_number_new(double value) {
  Obj *obj = obj_new();
  obj->kind = OBJ_NUMBER;
  obj->data.number = value;
  return obj;
}

Obj *obj_string_new(char *value, size_t len) {
  Obj *obj = obj_new();
  obj->kind = OBJ_STRING;
  obj->data.string.value = value;
  obj->data.string.len = len;
  return obj;
}
