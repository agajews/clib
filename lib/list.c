#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "listi.h"
#include "str.h"
#include "iter.h"

void list_init(List *l, String * (*to_string)(void *), void (*free_string)(String *),
               void (*free_data)(void *), int (*cmp)(const void *, const void *)) {
  l->head = NULL;
  l->tail = NULL;
  l->len = 0;
  l->to_string = to_string;
  l->free_string = free_string;
  l->free_data = free_data;
  l->cmp = cmp;
}

List * list_alloc(String * (*to_string)(void *), void (*free_string)(String *),
                  void (*free_data)(void *), int (*cmp)(const void *, const void *)) {
  List *l = malloc(sizeof(List));
  list_init(l, to_string, free_string, free_data, cmp);
  return l;
}

ListI * list_to_listi(List *l) {
  return listi_alloc(l, list_get_void, list_remove_void,
                     list_update_void, list_len_void,
                     l->to_string, l->free_string, list_iter_void, l->cmp);
}

void list_elem_init(ListElem *e) {
  e->next = NULL;
  e->data = NULL;
}

ListElem * list_elem_alloc() {
  ListElem *e = malloc(sizeof(ListElem));
  list_elem_init(e);
}

void list_elem_free(ListElem *e) {
  free(e);
}

void list_free(List *l) {
  IterState *is = iterstate_alloc();
  ListElem *elem = list_iter_elems(l, is);
  while (elem != NULL) {
    list_elem_free(elem);
    elem = list_iter_elems(l, is);
  }
  iterstate_free(is);
  free(l);
}

void list_free_data(List *l) {
  IterState *is = iterstate_alloc();
  void *data = list_iter(l, is);
  while (data != NULL) {
    l->free_data(data);
    data = list_iter(l, is);
  }
  iterstate_free(is);
}

void list_free_all(List *l) {
  list_free_data(l);
  list_free(l);
}

ListElem * list_get_elem(List *l, int idx) {
  if (idx == l->len - 1)
    return l->tail;
  ListElem *elem = l->head;
  int i = 0;
  while (i++ < idx)
    elem = elem->next;
  return elem;
}

void * list_iter_elems(List *l, IterState *is) {
  ListElem *curr;
  if (!iterstate_started(is)) {
    iterstate_start(is);
    curr = l->head;
  } else
    curr = as_list_elem(iterstate_state(is));
  if (curr != NULL)
    iterstate_set(is, curr->next);
  return curr;
}

void * list_iter_from(List *l, IterState *is, int start) {
  ListElem *curr;
  if (!iterstate_started(is)) {
    iterstate_start(is);
    curr = list_get_elem(l, start);
  } else
    curr = as_list_elem(iterstate_state(is))->next;
  iterstate_set(is, curr);
  if (curr == NULL)
    return NULL;
  return list_data(curr);
}

void * list_iter(List *l, IterState *is) {
  return list_iter_from(l, is, 0);
}

void * list_iter_void(void *l, IterState *is) {
  return list_iter(as_list(l), is);
}

List * as_list(void *l) {
  return (List *)l;
}

ListElem * as_list_elem(void *le) {
  return (ListElem *)le;
}

int list_len(List *l) {
  return l->len;
}

int list_len_void(void *l) {
  return list_len(as_list(l));
}

void * list_data(ListElem *e) {
  return e->data;
}

void * list_get(List *l, int idx) {
  return list_data(list_get_elem(l, idx));
}

void * list_get_void(void *l, int idx) {
  return list_get(as_list(l), idx);
}

void list_insert_elem_at(List *l, ListElem *prev, ListElem *curr) {
  (l->len)++;
  curr->next = prev->next;
  prev->next = curr;
  if (prev == l->tail)
    l->tail = curr;
}

void list_insert_elem(List *l, ListElem *e, int idx) {
  if (idx == 0) {
    ListElem *prev_head = l->head;
    l->head = e;
    if (l->tail == NULL)
      l->tail = e;
    e->next = prev_head;
    (l->len)++;
  } else {
    ListElem *prev = list_get_elem(l, idx - 1);
    list_insert_elem_at(l, prev, e);
  }
}

void list_insert(List *l, void *data, int idx) {
  ListElem *e = list_elem_alloc();
  e->data = data;
  list_insert_elem(l, e, idx);
}

void list_insert_void(void *l, void *data, int idx) {
  return list_insert(as_list(l), data, idx);
}

void list_append(List *l, void *data) {
  list_insert(l, data, l->len);
}

void list_prepend(List *l, void *data) {
  list_insert(l, data, 0);
}

void * list_update_elem(ListElem *e, void *new_data) {
  void *old_data = e->data;
  e->data = new_data;
  return old_data;
}

void * list_update(List *l, void *new_data, int idx) {
  ListElem *e = list_get(l, idx);
  return list_update_elem(e, new_data);
}

void * list_update_void(void *l, void *new_data, int idx) {
  return list_update(as_list(l), new_data, idx);
}

ListElem * list_remove_elem_at(List *l, ListElem *parent) {
  (l->len)--;
  ListElem *elem = parent->next;
  parent->next = elem->next;  // NULL if elem == l->tail
  if (elem == l->tail)
    l->tail = parent;
  return elem;
}

ListElem * list_remove_elem(List *l, int idx) {
  if (idx == 0) {
    (l->len)--;
    ListElem *e = l->head;
    l->head = e->next;
    if (l->head == NULL)
      l->tail = NULL;
    return e;
  }
  ListElem *e = list_remove_elem_at(l, list_get_elem(l, idx - 1));
  return e;
}

void * list_remove(List *l, int idx) {
  ListElem *e = list_remove_elem(l, idx);
  void *data = list_data(e);
  list_elem_free(e);
  return data;
}

void * list_remove_void(void *l, int idx) {
  return list_remove(as_list(l), idx);
}

String * list_to_string(List *l) {
  ListI *li = list_to_listi(l);
  String *s = listi_to_string(li);
  listi_free(li);
  return s;
}

List * list_sub(List *l, int start, int end) {
  List *res = list_alloc(l->to_string, l->free_string, l->free_data, l->cmp);
  for (int i = start; i < end; i++)
    list_append(res, list_get(l, i));
  return res;
}

