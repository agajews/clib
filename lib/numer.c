#include <stdio.h>
#include <stdlib.h>
#include "numer.h"

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
