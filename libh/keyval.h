#ifndef _KEYVAL_H_
#define _KEYVAL_H_

typedef struct String_ String;

typedef struct KeyVal_ {
  void *key;
  void *val;
  String * (*key_to_string)(void *key);
  String * (*val_to_string)(void *val);
  void (*free_key_string)(String *s);
  void (*free_val_string)(String *s);
  int (*cmp)(const void *, const void *);
} KeyVal;

void keyval_init(KeyVal *kv, void *key, void *val,
                 String * (*key_to_string)(void *), String * (*val_to_string)(void *),
                 void (*free_key_string)(String *), void (*free_val_string)(String *),
                 int (*cmp)(const void *, const void *));

KeyVal * keyval_alloc(void *key, void *val,
                      String * (*key_to_string)(void *), String * (*val_to_string)(void *),
                      void (*free_key_string)(String *), void (*free_val_string)(String *),
                      int (*cmp)(const void *, const void *));

void keyval_free(KeyVal *kv);

void keyval_free_void(void *kv);

KeyVal * as_keyval(void *kv);

void keyval_set_key(KeyVal *kv, void *key);

void keyval_set_val(KeyVal *kv, void *val);

void keyval_set_keyval(KeyVal *kv, void *key, void *val);

String * keyval_key_to_string(KeyVal *kv);

String * keyval_val_to_string(KeyVal *kv);

void keyval_free_key_string(KeyVal *kv, String *s);

void keyval_free_val_string(KeyVal *kv, String *s);

KeyVal * as_keyval_const(const void *kv);

int keyval_cmp(const void *a, const void *b);

String * keyval_to_string(KeyVal *kv);

String * keyval_to_string_void(void *kv);

#endif
