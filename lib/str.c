#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numer.h"
#include "str.h"
#include "list.h"
#include "listi.h"

void string_init(String *s) {
  s->str = NULL;
  s->len = 0;
}

String * string_alloc() {
  String *s = malloc(sizeof(String));
  string_init(s);
  return s;
}

String * string_alloc_str(int len) {
  String *s = string_alloc();
  s->str = malloc(sizeof(char) * (len + 1));
  s->len = len;
  string_set_null(s);
  return s;
}

void string_free(String *s) {
  free(s);
}

void string_free_str(String *s) {
  free(s->str);
  free(s);
}

void string_free_void(void *s) {
  string_free_str(as_string(s));
}

void string_set_str(String *s, char *str) {
  s->str = str;
  s->len = strlen(str);
}

void string_str_copy_to(String *dest, char *src, int start) {
  strcpy(dest->str + start, src);
}

void string_set_str_copy(String *dest, char *src) {
  string_str_copy_to(dest, src, 0);
}

void string_copy_to(String *dest, String *src, int start) {
  strcpy(dest->str + start, src->str);
}

void string_set_copy(String *dest, String *src) {
  string_copy_to(dest, src, 0);
  dest->len = src->len;
}

void string_copy_sub_to(String *dest, int dest_start, String *src, int start, int end) {
  strncpy(dest->str + dest_start, src->str + start, (end - start) * sizeof(char));
}

void string_copy_sub(String *dest, String *src, int start, int end) {
  string_copy_sub_to(dest, 0, src, start, end);
}

String * string_from_str(char *str) {
  String *s = string_alloc();
  string_set_str(s, str);
  return s;
}

String * string_from_str_copy(char *str) {
  String *s = string_alloc_str(strlen(str));
  string_set_str_copy(s, str);
  return s;
}

char * string_str(String *s) {
  return s->str;
}

int string_len(String *s) {
  return s->len;
}

void string_set_len(String *s, int len) {
  s->len = len;
}

void string_len_strlen(String *s) {
  string_set_len(s, strlen(string_str(s)));
}

String * string_copy(String *s) {
  String *c = string_alloc_str(string_len(s));
  string_set_copy(c, s);
  return c;
}

char string_get_char(String *s, int i) {
  return s->str[i];
}

void string_set_char(String *s, char c, int i) {
  s->str[i] = c;
}

void string_set_null(String *s) {
  string_set_char(s, '\0', string_len(s));
}

String * string_sub(String *s, int start, int end) {
  String *sub = string_alloc_str(end - start);
  string_copy_sub(sub, s, start, end);
  sub->len = end - start;
  return sub;
}

String * string_sub_from(String *s, int start) {
  return string_sub(s, start, s->len);
}

String * string_sub_to(String *s, int end) {
  return string_sub(s, 0, end);
}

int string_cmp_sub_sub(String *a, int a_start, int a_end,
                       String *b, int b_start, int b_end) {
  return strncmp((const char *)a->str + a_start, (const char *)b->str + b_start,
                 int_max(a_end - a_start, b_end - b_start));
}

int string_cmp_sub(String *a, String *b, int a_start, int a_end) {
  return string_cmp_sub_sub(a, a_start, a_end, b, 0, string_len(b));
}

int string_cmp(String *a, String *b) {
  return string_cmp_sub(a, b, 0, string_len(a));
}

int string_cmp_void(const void *a, const void *b) {
  return string_cmp(as_const_string(a), as_const_string(b));
}

int string_eq_sub_sub(String *a, int a_start, int a_end,
                      String *b, int b_start, int b_end) {
  int e = string_cmp_sub_sub(a, a_start, a_end, b, b_start, b_end);
  if (e == 0)
    return 1;
  return 0;
}

int string_eq_sub(String *a, String *b, int a_start, int a_end) {
  return string_eq_sub_sub(a, a_start, a_end, b, 0, string_len(b));
}

int string_eq(String *a, String *b) {
  return string_eq_sub(a, b, 0, string_len(a));
}

int string_eq_char(String *a, char c) {
  if (string_len(a) == 1 && string_str(a)[0] == c)
    return 1;
  return 0;
}

int string_count_char(String *s, char c) {
  int count = 0;
  for (int i = 0; i < s->len; i++)
    if (string_get_char(s, i) == c)
      count++;
  return count;
}

int string_count(String *s, String *c) {
  int count = 0, j = 0;
  String *sub = string_sub(s, j, j + string_len(c));
  while (j + string_len(c) <= string_len(s)) {
    while (j + string_len(c) <= string_len(s) &&
           string_eq_sub(s, c, j, j + string_len(c))) {
      j += string_len(c);
      count++;
    }
    while (j + string_len(c) <= string_len(s) &&
           !string_eq_sub(s, c, j, j + string_len(c)))
      j++;
  }
  string_free_str(sub);
  return count;
}

String * as_string(void *s) {
  return (String *)s;
}

String * as_const_string(const void *s) {
  return (String *)s;
}

String * string_to_string(void *s) {
  return (String *)s;
}

String * string_remove(String *s, String *r) {
  int c = string_count(s, r), i = 0, j = 0;
  String *res = string_alloc_str(string_len(s) - c * string_len(r));
  while (j + string_len(r) <= string_len(s)) {
    while (j + string_len(r) <= string_len(s) &&
           string_eq_sub(s, r, j, j + string_len(r)))
      j += string_len(r);
    int str_start = j;
    while (j + string_len(r) <= string_len(s)
           && !string_eq_sub(s, r, j, j + string_len(r)))
      j++;
    if (string_eq_sub(s, r, j, j + string_len(r)))
      string_copy_sub_to(res, i, s, str_start, j);
    else  // stopped at end of string
      string_copy_sub_to(res, i, s, str_start, string_len(s));
    i += j - str_start;
  }
  string_set_null(res);
  return res;
}

List * string_split(String *s, String *split) {
  int j = 0;
  List *strs = list_alloc(string_to_string, NULL, string_free_void);
  while (j + string_len(split) <= string_len(s)) {
    while (j + string_len(split) <= string_len(s) &&
           string_eq_sub(s, split, j, j + string_len(split)))
      j += string_len(split);
    int str_start = j;
    while (j + string_len(split) <= string_len(s)
           && !string_eq_sub(s, split, j, j + string_len(split)))
      j++;
    String *curr;
    if (string_eq_sub(s, split, j, j + string_len(split)))
      curr = string_sub(s, str_start, j);
    else  // stopped at end of string
      curr = string_sub_from(s, str_start);
    list_append(strs, curr);
  }
  return strs;
}

String * string_join(ListI *l, String *s) {
  if (listi_len(l) < 1)
    return string_alloc();
  int len = string_len(s) * (listi_len(l) - 1);
  for (int i = 0; i < listi_len(l); i++)
    len += string_len(as_string(listi_get(l, i)));
  String *str = string_alloc_str(len);
  int j = 0;
  for (int i = 0; i < listi_len(l) - 1; i++) {
    String *curr = as_string(listi_get(l, i));
    string_copy_to(str, curr, j);
    j += string_len(curr);
    string_copy_to(str, s, j);
    j += string_len(s);
  }
  String *curr = as_string(listi_get(l, listi_len(l) - 1));
  string_copy_to(str, curr, j);
  return str;
}
