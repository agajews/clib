#include <stdio.h>
#include <stdlib.h>
#include "listi.h"
#include "str.h"

void listi_init(ListI *l, void *list, void * (*get)(void *, int),
                void * (*remove)(void *, int),
                void * (*update)(void *, void *, int),
                int (*len)(void *),
                String * (*to_string)(void *),
                void (*free_string)(String *)) {
  l->list = list;
  l->get = get;
  l->remove = remove;
  l->update = update;
  l->len = len;
  l->to_string = to_string;
  l->free_string = free_string;
}

ListI * listi_alloc(void *list, void * (*get)(void *, int),
                    void * (*remove)(void *, int),
                    void * (*update)(void *, void *, int),
                    int (*len)(void *),
                    String * (*to_string)(void *),
                    void (*free_string)(String *)) {
  ListI *l = malloc(sizeof(ListI));
  listi_init(l, list, get, remove, update, len, to_string, free_string);
  return l;
}

void listi_free(ListI *l) {
  free(l);
}

void * listi_get(ListI *l, int idx) {
  return l->get(l->list, idx);
}

void * listi_remove(ListI *l, int idx) {
  return l->remove(l->list, idx);
}

void * listi_update(ListI *l, void *data, int idx) {
  return l->update(l->list, data, idx);
}

int listi_len(ListI *l) {
  return l->len(l->list);
}

String * listi_elem_to_string(ListI *l, void *data) {
  return l->to_string(data);
}

void listi_free_string(ListI *l, String *str) {
  if (l->free_string != NULL)
    l->free_string(str);
}

String * listi_to_string(ListI *l) {
  if (listi_len(l) == 0)
    return string_from_str_copy("[]");
  int len = 2 + 2 * (listi_len(l) - 1);
  for (int i = 0; i < listi_len(l); i++) {
    String *str = listi_elem_to_string(l, listi_get(l, i));
    len += string_len(str);
    listi_free_string(l, str);
  }
  String *str = string_alloc_str(len);
  int j = 0;
  string_set_char(str, '[', j++);
  for (int i = 0; i < listi_len(l) - 1; i++) {
    String *curr = listi_elem_to_string(l, listi_get(l, i));
    string_copy_to(str, curr, j);
    j += string_len(curr);
    listi_free_string(l, curr);
    string_set_char(str, ',', j++);
    string_set_char(str, ' ', j++);
  }
  String *curr = listi_elem_to_string(l, listi_get(l, listi_len(l) - 1));
  string_copy_to(str, curr, j);
  j += string_len(curr);
  listi_free_string(l, curr);
  string_set_char(str, ']', j++);
  string_set_null(str);
  return str;
}
