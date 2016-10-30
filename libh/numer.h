#ifndef _NUMER_H_
#define _NUMER_H_

typedef struct String_ String;

int int_cmp(const void *a, const void *b);

int int_max(int a, int b);

int int_min(int a, int b);

int * as_int_ptr(void *a);

int double_cmp(const void *a, const void *b);

String * double_to_string(double d);

String * double_to_string_void(void *d);

#endif
