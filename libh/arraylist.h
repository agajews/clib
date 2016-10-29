#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

typedef struct String_ String;

typedef struct ArrayList_ {
  void *arr;
  int len;
} ArrayList;

void arraylist_init(ArrayList *a);

void arraylist_set_arr(ArrayList *a, void *arr, int len);

int arraylist_len(ArrayList *a);

void * arraylist_arr(ArrayList *a);

ArrayList * arraylist_alloc();

ArrayList * arraylist_alloc_arr(int len);

void arraylist_free(ArrayList *a);

void arraylist_free_arr(ArrayList *a);

void arraylist_free_cnt(ArrayList *a, void (*free_fn)(void *));

void arraylist_free_all(ArrayList *a, void (*free_fn)(void *));

void * arraylist_get(ArrayList *a, int i);

void arraylist_set(ArrayList *a, int i, void *data);

String * arraylist_to_string(ArrayList *a, String * (*to_string)(void *),
                             void (*free_fn)(String *));

#endif
