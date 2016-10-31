#include <stdio.h>
#include <stdlib.h>
#include "listi.h"
#include "str.h"

void listi_init(ListI *l, void *list, void * (*get)(void *, int),
                void * (*remove)(void *, int),
                void * (*update)(void *, void *, int),
                int (*len)(void *),
                String * (*to_string)(void *),
                void (*free_string)(String *),
                void * (*iter_from)(void *, IterState *, int),
                int (*cmp)(const void *, const void *)) {
  l->list = list;
  l->get = get;
  l->remove = remove;
  l->update = update;
  l->len = len;
  l->to_string = to_string;
  l->free_string = free_string;
  l->iter_from = iter_from;
  l->cmp = cmp;
}

ListI * listi_alloc(void *list, void * (*get)(void *, int),
                    void * (*remove)(void *, int),
                    void * (*update)(void *, void *, int),
                    int (*len)(void *),
                    String * (*to_string)(void *),
                    void (*free_string)(String *),
                    void * (*iter_from)(void *, IterState *, int),
                    int (*cmp)(const void *, const void *)) {
  ListI *l = malloc(sizeof(ListI));
  listi_init(l, list, get, remove, update, len, to_string, free_string, iter_from, cmp);
  return l;
}

void listi_free(ListI *l) {
  free(l);
}

void * listi_get(ListI *l, int idx) {
  return l->get(l->list, idx);
}

void * listi_iter_from(ListI *l, IterState *is, int start) {
  return l->iter_from(l->list, is, start);
}

void * listi_iter(ListI *l, IterState *is) {
  return l->iter_from(l->list, is, 0);
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

int listi_cmp(ListI *l, const void *a, const void *b) {
  return l->cmp(a, b);
}

int listi_bin_search(ListI *l, void *key) {
  int lo = 0, hi = listi_len(l) - 1;
  int mid = lo + (hi - lo) / 2;
  int cmp_val = listi_cmp(l, key, listi_get(l, mid));
  while (cmp_val != 0) {
    if (cmp_val < 0)
      hi = mid;
    else if (cmp_val > 0)
      lo = mid;
    mid = lo + (hi - lo) / 2;
    cmp_val = listi_cmp(l, key, listi_get(l, mid));
    if (lo == mid)
      break;
  }
  if (cmp_val != 0)
    return mid + 1;
  return mid;
}

ListI * as_listi(void *l) {
  return (ListI *)l;
}

/* int listi_sort_cmp(const void *a, const void *b) { */
/*   int ai = *((int *)a); */
/*   int bi = *((int *)b); */
/*   ListI *li = as_listi(l); */
/*   return listi_cmp(li, listi_get(li, ai), listi_get(li, bi)); */
/* } */

/* void listi_sort(ListI *l) { */
/*   int **elems = malloc(listi_len(l) * sizeof(void *));  // actually is void ** */
/*   int *indices = malloc(listi_len(l) * sizeof(int)); */
/*   for (int i = 0; i < listi_len(l); i++) { */
/*     indices[i] = i; */
/*     elems[i] = listi_get(l, i); */
/*   } */
/*   qsort_r(indices, listi_len(l), sizeof(int), listi_sort_cmp, l); */
/*   for (int i = 0; i < listi_len(l); i++) */
/*     listi_update(l, elems[indices[i]], i); */
/*   free(indices); */
/*   free(elems); */
/* } */
