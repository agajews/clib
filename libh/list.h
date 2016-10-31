#ifndef _LIST_H_
#define _LIST_H_

typedef struct String_ String;

typedef struct IterState_ IterState;

typedef struct ListI_ ListI;

typedef struct ListElem_ {
  struct ListElem_ *next;
  void *data;
} ListElem;

typedef struct List_ {
  ListElem *head;
  ListElem *tail;
  int len;
  String * (*to_string)(void *);
  void (*free_string)(String *);
  void (*free_data)(void *);
  int (*cmp)(const void *, const void *);
} List;

void list_init(List *l, String * (*to_string)(void *), void (*free_string)(String *),
               void (*free_data)(void *), int (*cmp)(const void *, const void *));

List * list_alloc(String * (*to_string)(void *), void (*free_string)(String *),
                  void (*free_data)(void *), int (*cmp)(const void *, const void *));

List * list_bare_alloc();

ListI * list_to_listi(List *l);

void list_elem_init(ListElem *e);

ListElem * list_elem_alloc();

void list_elem_free(ListElem *e);

void list_free(List *l);

void list_free_data(List *l);

void list_free_all(List *l);

ListElem * list_get_elem(List *l, int idx);

void * list_iter_from(List *l, IterState *is, int start);

void * list_iter_from_void(void *l, IterState *is, int start);

void * list_iter(List *l, IterState *is);

void * list_iter_elems(List *l, IterState *is);

void * list_iter_void(void *l, IterState *is);

List * as_list(void *l);

ListElem * as_list_elem(void *le);

int list_len(List *l);

int list_len_void(void *l);

void * list_data(ListElem *e);

void * list_get(List *l, int idx);

void list_insert_elem_at(List *l, ListElem *prev, ListElem *curr);

void list_insert_elem(List *l, ListElem *e, int idx);

void list_insert(List *l, void *data, int idx);

void * list_get_void(void *l, int idx);

void list_insert_void(void *l, void *data, int idx);

void list_append(List *l, void *data);

void list_prepend(List *l, void *data);

void * list_update_elem(ListElem *e, void *new_data);

void * list_update(List *l, void *new_data, int idx);

void * list_update_void(void *l, void *new_data, int idx);

ListElem * list_remove_elem_at(List *l, ListElem *parent);

ListElem * list_remove_elem(List *l, int idx);

void * list_remove(List *l, int idx);

void * list_remove_void(void *l, int idx);

String * list_to_string(List *l);

List * list_sub(List *l, int start, int end);

#endif
