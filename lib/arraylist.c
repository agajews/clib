#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"
#include "str.h"

void arraylist_init(ArrayList *a) {
  a->arr = NULL;
  a->len = 0;
}

void arraylist_set_arr(ArrayList *a, void *arr, int len) {
  a->arr = arr;
  a->len = len;
}

int arraylist_len(ArrayList *a) {
  return a->len;
}

void * arraylist_arr(ArrayList *a) {
  return a->arr;
}

ArrayList * arraylist_alloc() {
  ArrayList *a = malloc(sizeof(ArrayList));
  arraylist_init(a);
  return a;
}

ArrayList * arraylist_alloc_arr(int len) {
  ArrayList *a = arraylist_alloc();
  a->arr = malloc(len * sizeof(void *));
  a->len = len;
  return a;
}

void arraylist_free(ArrayList *a) {
  free(a);
}

void arraylist_free_arr(ArrayList *a) {
  free(a->arr);
  free(a);
}

void arraylist_free_cnt(ArrayList *a, void (*free_fn)(void *)) {
  for (int i = 0; i < a->len; i++)
    free_fn(arraylist_get(a, i));
}

void arraylist_free_all(ArrayList *a, void (*free_fn)(void *)) {
  arraylist_free_cnt(a, free_fn);
  arraylist_free_arr(a);
}

void * arraylist_get(ArrayList *a, int i) {
  return ((int **)a->arr)[i];
}

void arraylist_set(ArrayList *a, int i, void *data) {
  ((int **)a->arr)[i] = (int *)data;
}

String * arraylist_to_string(ArrayList *a, String * (*to_string)(void *),
                             void (*free_fn)(String *)) {
  if (a->len == 0)
    return string_from_str_copy("[]");
  int len = 2 + 2 * (a->len - 1);
  for (int i = 0; i < a->len; i++) {
    String *str = to_string(arraylist_get(a, i));
    len += string_len(str);
    if (free_fn != NULL)
      free_fn(str);
  }
  String *str = string_alloc_str(len);
  int j = 0;
  string_set_char(str, '[', j++);
  for (int i = 0; i < a->len - 1; i++) {
    String *curr = to_string(arraylist_get(a, i));
    string_copy_to(str, curr, j);
    j += curr->len;
    if (free_fn != NULL)
      free_fn(curr);
    string_set_char(str, ',', j++);
    string_set_char(str, ' ', j++);
  }
  String *curr = to_string(arraylist_get(a, a->len - 1));
  string_copy_to(str, curr, j);
  j += curr->len;
  if (free_fn != NULL)
    free_fn(curr);
  string_set_char(str, ']', j++);
  string_set_null(str);
  return str;
}
