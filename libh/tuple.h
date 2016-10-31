#ifndef _TUPLE_H_
#define _TUPLE_H_

typedef struct Tuple_ {
  int size;
  void *elems;
} Tuple;

Tuple * tuple_alloc(int size);

void tuple_free(Tuple *tuple);

void * tuple_get(Tuple *tuple, int i);

void tuple_set(Tuple *tuple, int i, void *data);

#endif
