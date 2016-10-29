#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "numer.h"

int main(int argc, char **argv) {
  // Creating
  Tree *tree = tree_alloc();
  tree_init(tree, int_cmp);

  // Empty finding
  int dummy = 1, *found;
  int res = tree_find(tree, &dummy, &found);
  printf("Res: %d\n", res);

  // Inserting
  int *key = malloc(sizeof(int)), *val = malloc(sizeof(int));
  *key = 10; *val = 20;
  tree_insert(tree, key, val);
  int key2 = 30, val2 = 42;
  tree_insert(tree, &key2, &val2);
  int key3 = 30, val3 = 43;
  tree_insert(tree, &key3, &val3);
  int key4 = 5, val4 = 4;
  tree_insert(tree, &key4, &val4);

  // Finding
  res = tree_find(tree, key, &found);
  printf("Key: %d, Res: %d, found: %d\n", *key, res, *found);
  res = tree_find(tree, &key2, &found);
  printf("Key: %d, Res: %d, found: %d\n", key2, res, *found);
  res = tree_find(tree, &key3, &found);
  printf("Key: %d, Res: %d, found: %d\n", key3, res, *found);
  res = tree_find(tree, &key4, &found);
  printf("Key: %d, Res: %d, found: %d\n", key4, res, *found);

  // Removing
  res = tree_remove(tree, key, &found, NULL);
  printf("Removed key: %d Res: %d, found: %d\n", *key, res, *found);
  *found = 0;

  // Finding
  res = tree_find(tree, key, &found);
  printf("Key: %d, Res: %d, found: %d\n", *key, res, *found);
  res = tree_find(tree, &key2, &found);
  printf("Key: %d, Res: %d, found: %d\n", key2, res, *found);
  res = tree_find(tree, &key3, &found);
  printf("Key: %d, Res: %d, found: %d\n", key3, res, *found);
  res = tree_find(tree, &key4, &found);
  printf("Key: %d, Res: %d, found: %d\n", key4, res, *found);

  // Updating
  int val1_2 = 50;
  *found = 0;
  res = tree_update(tree, key, &val1_2, &found);
  printf("Updated key: %d Res: %d, found: %d\n", *key, res, *found);
  res = tree_update(tree, &key2, &val1_2, &found);
  printf("Updated key: %d Res: %d, found: %d\n", key2, res, *found);

  // Finding
  res = tree_find(tree, key, &found);
  printf("Key: %d, Res: %d, found: %d\n", *key, res, *found);
  res = tree_find(tree, &key2, &found);
  printf("Key: %d, Res: %d, found: %d\n", key2, res, *found);
  res = tree_find(tree, &key3, &found);
  printf("Key: %d, Res: %d, found: %d\n", key3, res, *found);
  res = tree_find(tree, &key4, &found);
  printf("Key: %d, Res: %d, found: %d\n", key4, res, *found);

  tree_free(tree);
  free(key); free(val);
  return 0;
}
