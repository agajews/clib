#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "arraylist.h"

#define MAXLEN 1000

int main(int argc, char **argv) {
  String *s = string_alloc_str(MAXLEN);
  String *space = string_from_str("  ");
  String *nl = string_from_str("\n");
  String *hey = string_from_str("hey");
  while (1) {
    fgets(string_str(s), MAXLEN, stdin);
    string_len_strlen(s);
    String *clean = string_remove(s, nl);
    String *l = string_remove(clean, hey);
    string_free_str(clean);
    printf("Read: %s\n", string_str(l));
    ArrayList *strs = string_split(l, space);
    String *pr = arraylist_to_string(strs, string_to_string);
    printf("Split: %s\n", string_str(pr));
    string_free_str(pr);
    string_free_str(l);
    arraylist_free_all(strs, string_free_void);
  }
  string_free(space);
  string_free_str(s);
  return 0;
}
