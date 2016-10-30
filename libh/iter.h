#ifndef _ITER_H_
#define _ITER_H_

typedef struct IterState_ {
  void *state;
  int started;
} IterState;

void iterstate_init(IterState *is);

IterState * iterstate_alloc();

void iterstate_free(IterState *is);

void * iterstate_state(IterState *is);

int iterstate_started(IterState *is);

void iterstate_start(IterState *is);

void iterstate_set(IterState *is, void *state);

#endif
