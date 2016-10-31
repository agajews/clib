#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "list.h"

void stack_init(Stack *s, void (*free_data)(void *),
                int (*cmp)(const void *, const void *)) {
  return list_init(s, NULL, NULL, free_data, cmp);
}

Stack * stack_alloc(void (*free_data)(void *),
                    int (*cmp)(const void *, const void *)) {
  return list_alloc(NULL, NULL, free_data, cmp);
}

void stack_free(Stack *s) {
  return list_free(s);
}

void stack_free_data(Stack *s) {
  return list_free_data(s);
}

void stack_free_all(Stack *s) {
  return list_free_all(s);
}

Stack * as_stack(void *s) {
  return (Stack *)s;
}

int stack_height(Stack *s) {
  return list_len(s);
}

void stack_push(Stack *s, void *data) {
  list_append(s, data);
}

void * stack_peek(Stack *s) {
  return list_get(s, list_len(s) - 1);
}

void * stack_pop(Stack *s) {
  return list_remove(s, list_len(s) - 1);
}

int stack_is_empty(Stack *s) {
  if (stack_height(s) == 0)
    return 1;
  return 0;
}
