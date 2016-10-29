#ifndef _STRING_H_
#define _STRING_H_

typedef struct List_ List;
typedef struct ListI_ ListI;

typedef struct String_ {
  char *str;
  int len;
} String;

void string_init(String *s);

String * string_alloc();

String * string_alloc_str(int len);

void string_free(String *s);

void string_free_str(String *s);

void string_free_void(void *s);

void string_set_str(String *s, char *str);

void string_str_copy_to(String *dest, char *src, int start);

void string_set_str_copy(String *dest, char *src);

void string_copy_to(String *dest, String *src, int start);

void string_set_copy(String *dest, String *src);

void string_copy_sub_to(String *dest, int dest_start, String *src, int start, int end);

void string_copy_sub(String *dest, String *src, int start, int end);

String * string_from_str(char *str);

String * string_from_str_copy(char *str);

char * string_str(String *s);

int string_len(String *s);

void string_set_len(String *s, int len);

void string_len_strlen(String *s);

String * string_copy(String *s);

char string_get_char(String *s, int i);

void string_set_char(String *s, char c, int i);

void string_set_null(String *s);

String * string_sub(String *s, int start, int end);

String * string_sub_from(String *s, int start);

String * string_sub_to(String *s, int end);

int string_cmp_sub_sub(String *a, int a_start, int a_end,
                       String *b, int b_start, int b_end);

int string_cmp_sub(String *a, String *b, int a_start, int a_end);

int string_cmp(String *a, String *b);

int string_cmp_void(const void *a, const void *b);

int string_eq_sub_sub(String *a, int a_start, int a_end,
                      String *b, int b_start, int b_end);

int string_eq_sub(String *a, String *b, int a_start, int a_end);

int string_eq(String *a, String *b);

int string_eq_char(String *a, char c);

int string_count_char(String *s, char c);

int string_count(String *s, String *c);

String * as_string(void *s);

String * as_const_string(const void *s);

String * string_to_string(void *s);

String * string_remove(String *s, String *r);

List * string_split(String *s, String *split);

String * string_join(ListI *a, String *s);

#endif
