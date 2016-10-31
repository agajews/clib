#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "listi.h"
#include "iter.h"
#include "numer.h"
#include "str.h"

void dblarray_init(DblArray *a) {
  a->arr = NULL;
  a->len = 0;
}

DblArray * as_dblarray(void *a) {
  return (DblArray *)a;
}

void dblarray_set_arr(DblArray *a, double *arr, int len) {
  a->arr = arr;
  a->len = len;
}

int dblarray_len(DblArray *a) {
  return a->len;
}

int dblarray_len_void(void *a) {
  return dblarray_len(as_dblarray(a));
}

double * dblarray_arr(DblArray *a) {
  return a->arr;
}

DblArray * dblarray_alloc() {
  DblArray *a = malloc(sizeof(DblArray));
  dblarray_init(a);
  return a;
}

DblArray * dblarray_alloc_arr(int len) {
  DblArray *a = dblarray_alloc();
  a->arr = malloc(len * sizeof(void *));
  a->len = len;
  return a;
}

void dblarray_free(DblArray *a) {
  free(a);
}

void dblarray_free_void(void *a) {
  return dblarray_free(as_dblarray(a));
}

void dblarray_free_arr(DblArray *a) {
  free(a->arr);
  free(a);
}

void dblarray_free_arr_void(void *a) {
  return dblarray_free_arr(as_dblarray(a));
}

double dblarray_get(DblArray *a, int i) {
  return a->arr[i];
}

void * dblarray_get_void(void *a, int i) {
  return as_dblarray(a)->arr + i;
}

void dblarray_set(DblArray *a, double data, int i) {
  a->arr[i] = data;
}

void * dblarray_set_void(void *a, void *data, int i) {
  as_dblarray(a)->arr[i] = *((double *)data);
  return NULL;
}

double * dblarray_iter_from(DblArray *a, IterState *is, int start) {
  if (!iterstate_started(is)) {
    int *i = malloc(sizeof(int));
    iterstate_set(is, i);
    *i = start;
  }
  int *i = as_int_ptr(iterstate_state(is));
  if (*i == a->len)
    return NULL;
  return a->arr + (*i)++;
}

void * dblarray_iter_from_void(void *a, IterState *is, int start) {
  return dblarray_iter_from(as_dblarray(a), is, start);
}

double * dblarray_iter(DblArray *a, IterState *is) {
  return dblarray_iter_from(a, is, 0);
}

void * dblarray_iter_void(void *a, IterState *is) {
  return dblarray_iter_from_void(a, is, 0);
}

ListI * dblarray_to_listi(DblArray *a) {
  ListI *li = listi_alloc(a, dblarray_get_void, NULL, dblarray_set_void,
                          dblarray_len_void, double_to_string_void,
                          string_free_str,
                          dblarray_iter_from_void, double_cmp);
  return li;
}

double dblarray_min(DblArray *a) {
  double min = dblarray_get(a, 0);
  for (int i = 1; i < dblarray_len(a); i++)
    min = double_min(min, dblarray_get(a, i));
  return min;
}

double dblarray_max(DblArray *a) {
  double max = dblarray_get(a, 0);
  for (int i = 1; i < dblarray_len(a); i++)
    max = double_max(max, dblarray_get(a, i));
  return max;
}

int dblarray_bin_search(DblArray *a, double key) {
  ListI *li = dblarray_to_listi(a);
  int i = listi_bin_search(li, &key);
  listi_free(li);
  return i;
}

void dblarray_sort(DblArray *a) {
  qsort(a->arr, a->len, sizeof(double), double_cmp);
}

String * dblarray_to_string(DblArray *a) {
  ListI *li = dblarray_to_listi(a);
  String *s = listi_to_string(li);
  listi_free(li);
  return s;
}
