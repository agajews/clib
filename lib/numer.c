#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "numer.h"
#include "str.h"

#define MAX_DOUBLE_STR 20

int int_cmp(const void *a, const void *b) {
  int ai = *((int *)a);
  int bi = *((int *)b);
  if (ai > bi)
    return 1;
  else if (ai < bi)
    return -1;
  return 0;
}

int int_max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

int int_min(int a, int b) {
  if (a < b)
    return a;
  return b;
}

int * as_int_ptr(void *a) {
  return (int *)a;
}

int double_cmp(const void *a, const void *b) {
  double ad = *((double *)a);
  double bd = *((double *)b);
  if (ad > bd)
    return 1;
  else if (ad < bd)
    return -1;
  return 0;
}

double double_max(double a, double b) {
  if (a > b)
    return a;
  return b;
}

double double_min(double a, double b) {
  if (a < b)
    return a;
  return b;
}

String * double_to_string(double d) {
  String *str = string_alloc_str(MAX_DOUBLE_STR);
  snprintf(string_str(str), MAX_DOUBLE_STR * sizeof(char), "%f", d);
  string_len_strlen(str);
  return str;
}

String * double_to_string_void(void *d) {
  return double_to_string(*((double *)d));
}

void rand_seed() {
  srand(time(NULL));
}

double rand_double() {
  return (double)rand() / RAND_MAX;
}

double rand_range(double min, double max) {
  return min + rand_double() * (max - min);
}
