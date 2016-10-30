#ifndef _LISTI_H_
#define _LISTI_H_

typedef struct String_ String;

typedef struct IterState_ IterState;

typedef struct ListI_ {
  void *list;
  void * (*get)(void *, int);
  void * (*remove)(void *, int);
  void * (*update)(void *, void *, int);
  int (*len)(void *);
  String * (*to_string)(void *);
  void (*free_string)(String *);
  void * (*iter_from)(void *, IterState *, int);
  int (*cmp)(const void *, const void *);
} ListI;

void listi_init(ListI *l, void *list, void * (*get)(void *, int),
                void * (*remove)(void *, int),
                void * (*update)(void *, void *, int),
                int (*len)(void *),
                String * (*to_string)(void *),
                void (*free_string)(String *),
                void * (*iter_from)(void *, IterState *, int),
                int (*cmp)(const void *, const void *));

ListI * listi_alloc(void *list, void * (*get)(void *, int),
                    void * (*remove)(void *, int),
                    void * (*update)(void *, void *, int),
                    int (*len)(void *),
                    String * (*to_string)(void *),
                    void (*free_string)(String *),
                    void * (*iter_from)(void *, IterState *, int),
                    int (*cmp)(const void *, const void *));

void listi_free(ListI *l);

void * listi_get(ListI *l, int idx);

void * listi_iter_from(ListI *l, IterState *is, int start);

void * listi_iter(ListI *l, IterState *is);

void * listi_remove(ListI *l, int idx);

void * listi_update(ListI *l, void *data, int idx);

int listi_len(ListI *l);

String * listi_elem_to_string(ListI *l, void *data);

void listi_free_string(ListI *l, String *str);

String * listi_to_string(ListI *l);

int listi_cmp(ListI *l, const void *a, const void *b);

int listi_bin_search(ListI *l, void *key);

#endif
