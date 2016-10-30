#ifndef _ARRAY_H_
#define _ARRAY_H_

typedef struct IterState_ IterState;

typedef struct ListI_ ListI;

typedef struct DblArray_ {
  double *arr;
  int len;
} DblArray;

void dblarray_init(DblArray *a);

DblArray * as_dblarray(void *a);

void dblarray_set_arr(DblArray *a, double *arr, int len);

int dblarray_len(DblArray *a);

int dblarray_len_void(void *a);

double * dblarray_arr(DblArray *a);

DblArray * dblarray_alloc();

DblArray * dblarray_alloc_arr(int len);

void dblarray_free(DblArray *a);

void dblarray_free_void(void *a);

void dblarray_free_arr(DblArray *a);

void dblarray_free_arr_void(void *a);

double dblarray_get(DblArray *a, int i);

void * dblarray_get_void(void *a, int i);

void dblarray_set(DblArray *a, double data, int i);

void * dblarray_set_void(void *a, void *data, int i);

double * dblarray_iter(DblArray *a, IterState *is);

void * dblarray_iter_void(void *v, IterState *is);

ListI * dblarray_to_listi(DblArray *a);

#endif
