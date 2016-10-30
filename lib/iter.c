#include <stdio.h>
#include <stdlib.h>
#include "iter.h"

void iterstate_init(IterState *is) {
  is->state = NULL;
  is->started = 0;
}

IterState * iterstate_alloc() {
  IterState *is = malloc(sizeof(IterState));
  iterstate_init(is);
  return is;
}

void iterstate_free(IterState *is) {
  free(is);
}

void * iterstate_state(IterState *is) {
  return is->state;
}

void iterstate_start(IterState *is) {
  is->started = 1;
}

int iterstate_started(IterState *is) {
  return is->started;
}

void iterstate_set(IterState *is, void *state) {
  is->state = state;
}
