#include <stdio.h>
#include <stdlib.h>
#include "tuple.h"

Tuple * tuple_alloc(int size) {
  Tuple *tuple = malloc(sizeof(Tuple));
  tuple->size = size;
  tuple->elems = malloc(size * sizeof(void *));
  return tuple;
}

void tuple_free(Tuple *tuple) {
  free(tuple->elems);
  free(tuple);
}

void * tuple_get(Tuple *tuple, int i) {
  return ((int **)tuple->elems)[i];
}

void tuple_set(Tuple *tuple, int i, void *data) {
  ((int **)tuple->elems)[i] = data;
}
