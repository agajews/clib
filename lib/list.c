#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "listi.h"
#include "str.h"

void list_init(List *l, String * (*to_string)(void *), void (*free_string)(String *),
               void (*free_data)(void *)) {
  l->head = NULL;
  l->tail = NULL;
  l->len = 0;
  l->to_string = to_string;
  l->free_string = free_string;
  l->free_data = free_data;
}

List * list_alloc(String * (*to_string)(void *), void (*free_string)(String *),
                  void (*free_data)(void *)) {
  List *l = malloc(sizeof(List));
  list_init(l, to_string, free_string, free_data);
  return l;
}

ListI * list_to_listi(List *l) {
  return listi_alloc(l, list_get_void, list_remove_void,
                     list_update_void, list_len_void,
                     l->to_string, l->free_string);
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
  for (int i = l->len - 1; i >= 0; i--)
    list_elem_free(list_get_elem(l, i));
  free(l);
}

void list_free_data(List *l) {
  for (int i = 0; i < list_len(l); i++)
    l->free_data(list_get(l, i));
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

List * as_list(void *l) {
  return (List *)l;
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
  return list_data(e);
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
  List *res = list_alloc(l->to_string, l->free_string, l->free_data);
  for (int i = start; i < end; i++)
    list_append(res, list_get(l, i));
  return res;
}

