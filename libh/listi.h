#ifndef _LISTI_H_
#define _LISTI_H_

typedef struct String_ String;

typedef struct ListI_ {
  void *list;
  void * (*get)(void *, int);
  void * (*remove)(void *, int);
  void * (*update)(void *, void *, int);
  int (*len)(void *);
  String * (*to_string)(void *);
  void (*free_string)(String *);
} ListI;

void listi_init(ListI *l, void *list, void * (*get)(void *, int),
                void * (*remove)(void *, int),
                void * (*update)(void *, void *, int),
                int (*len)(void *),
                String * (*to_string)(void *),
                void (*free_string)(String *));

ListI * listi_alloc(void *list, void * (*get)(void *, int),
                    void * (*remove)(void *, int),
                    void * (*update)(void *, void *, int),
                    int (*len)(void *),
                    String * (*to_string)(void *),
                    void (*free_string)(String *));

void listi_free(ListI *l);

void * listi_get(ListI *l, int idx);

void * listi_remove(ListI *l, int idx);

void * listi_update(ListI *l, void *data, int idx);

int listi_len(ListI *l);

String * listi_elem_to_string(ListI *l, void *data);

void listi_free_string(ListI *l, String *str);

String * listi_to_string(ListI *l);

#endif
