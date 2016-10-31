#ifndef _STACK_H_
#define _STACK_H_

typedef struct List_ List;

typedef List Stack;

void stack_init(Stack *s, void (*free_data)(void *),
                int (*cmp)(const void *, const void *));

Stack * stack_alloc(void (*free_data)(void *),
                    int (*cmp)(const void *, const void *));

void stack_free(Stack *s);

void stack_free_data(Stack *s);

void stack_free_all(Stack *s);

Stack * as_stack(void *s);

int stack_height(Stack *s);

void stack_push(Stack *s, void *data);

void * stack_peek(Stack *s);

void * stack_pop(Stack *s);

int stack_is_empty(Stack *s);

#endif
