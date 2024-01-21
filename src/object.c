#include <stdio.h>
#include <stdlib.h>

#include "object.h"

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
