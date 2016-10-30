#include <stdio.h>
#include <stdlib.h>
#include "keyval.h"
#include "str.h"


void keyval_init(KeyVal *kv, void *key, void *val,
                 String * (*key_to_string)(void *), String * (*val_to_string)(void *),
                 void (*free_key_string)(String *), void (*free_val_string)(String *),
                 int (*cmp)(const void *, const void *)) {
  kv->key = key;
  kv->val = val;
  kv->key_to_string = key_to_string;
  kv->val_to_string = val_to_string;
  kv->free_key_string = free_key_string;
  kv->free_val_string = free_val_string;
  kv->cmp = cmp;
}


KeyVal * keyval_alloc(void *key, void *val,
                      String * (*key_to_string)(void *), String * (*val_to_string)(void *),
                      void (*free_key_string)(String *), void (*free_val_string)(String *),
                      int (*cmp)(const void *, const void *)) {
  KeyVal *kv = malloc(sizeof(KeyVal));
  keyval_init(kv, key, val, key_to_string, val_to_string,
              free_key_string, free_val_string, cmp);
  return kv;
}

void keyval_free(KeyVal *kv) {
  free(kv);
}

void keyval_free_void(void *kv) {
  keyval_free(as_keyval(kv));
}

KeyVal * as_keyval(void *kv) {
  return (KeyVal *)kv;
}

void keyval_set_key(KeyVal *kv, void *key) {
  kv->key = key;
}

void keyval_set_val(KeyVal *kv, void *val) {
  kv->val = val;
}

void keyval_set_keyval(KeyVal *kv, void *key, void *val) {
  keyval_set_key(kv, key);
  keyval_set_val(kv, val);
}

String * keyval_key_to_string(KeyVal *kv) {
  return kv->key_to_string(kv->key);
}

String * keyval_val_to_string(KeyVal *kv) {
  return kv->val_to_string(kv->val);
}

void keyval_free_key_string(KeyVal *kv, String *s) {
  if (kv->free_key_string != NULL)
    kv->free_key_string(s);
}

void keyval_free_val_string(KeyVal *kv, String *s) {
  if (kv->free_val_string != NULL)
    kv->free_val_string(s);
}

KeyVal * as_keyval_const(const void *kv) {
  return (KeyVal *)kv;
}

int keyval_cmp(const void *a, const void *b) {
  KeyVal *akv = as_keyval_const(a);
  KeyVal *bkv = as_keyval_const(b);
  return akv->cmp(akv->key, bkv->key);
}

String * keyval_to_string(KeyVal *kv) {
  String *key_str = keyval_key_to_string(kv);
  String *sep = string_from_str(":");
  String *val_str = keyval_val_to_string(kv);
  String *str = string_alloc_str(string_len(key_str) + string_len(val_str) +
                                 string_len(sep));
  string_copy_to(str, key_str, 0);
  string_copy_to(str, sep, string_len(key_str));
  string_copy_to(str, val_str, string_len(key_str) + string_len(sep));
  string_free(sep);
  keyval_free_key_string(kv, key_str);
  keyval_free_val_string(kv, val_str);
  return str;
}

String * keyval_to_string_void(void *kv) {
  return keyval_to_string(as_keyval(kv));
}
