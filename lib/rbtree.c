#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include "str.h"
#include "list.h"
#include "keyval.h"

void rbtree_init(RBTree *tree, int (*cmp)(const void *, const void *),
                 String * (*key_to_string)(void *), String * (*data_to_string)(void *),
                 void (*free_key_string)(String *), void (*free_data_string)(String *),
                 void (*free_key)(void *), void (*free_data)(void *)) {
  tree->cmp = cmp;
  tree->key_to_string = key_to_string;
  tree->data_to_string = data_to_string;
  tree->free_key_string = free_key_string;
  tree->free_data_string = free_data_string;
  tree->free_key = free_key;
  tree->free_data = free_data;
  tree->root = NULL;
  return;
}

RBTree * rbtree_alloc(int (*cmp)(const void *, const void *),
                      String * (*key_to_string)(void *), String * (*data_to_string)(void *),
                      void (*free_key_string)(String *), void (*free_data_string)(String *),
                      void (*free_key)(void *), void (*free_data)(void *)) {
  RBTree *tree = malloc(sizeof(RBTree));
  rbtree_init(tree, cmp, key_to_string, data_to_string, free_key_string, free_data_string,
              free_key, free_data);
  return tree;
}

void rbtree_init_node(RBNode *node) {
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  node->key = NULL;
  node->data = NULL;
  node->color = 1;
}

RBNode * rbtree_alloc_node() {
  RBNode *node = malloc(sizeof(RBNode));
  rbtree_init_node(node);
  return node;
}

void rbtree_free_node(RBNode *node) {
  if (node == NULL)
    return;
  free(node);
}

void rbtree_free_node_key(RBTree *tree, RBNode *node) {
  if (node == NULL)
    return;
  tree->free_key(node->key);
}

void rbtree_free_node_data(RBTree *tree, RBNode *node) {
  if (node == NULL)
    return;
  tree->free_data(node->data);
}

void rbtree_free_nodes(RBNode *node) {
  if (node == NULL)
    return;
  rbtree_free_nodes(node->left);
  rbtree_free_nodes(node->right);
  rbtree_free_node(node);
}

void rbtree_free_nodes_keys(RBTree *tree, RBNode *node) {
  if (node == NULL)
    return;
  rbtree_free_nodes_keys(tree, node->left);
  rbtree_free_nodes_keys(tree, node->right);
  rbtree_free_node_key(tree, node);
}

void rbtree_free_nodes_data(RBTree *tree, RBNode *node) {
  if (node == NULL)
    return;
  rbtree_free_nodes_data(tree, node->left);
  rbtree_free_nodes_data(tree, node->right);
  rbtree_free_node_data(tree, node);
}

void rbtree_free(RBTree *tree) {
  rbtree_free_nodes(tree->root);
  free(tree);
}

void rbtree_free_keys(RBTree *tree) {
  rbtree_free_nodes_keys(tree, tree->root);
}

void rbtree_free_data(RBTree *tree) {
  rbtree_free_nodes_data(tree, tree->root);
}

void rbtree_free_all(RBTree *tree) {
  rbtree_free_keys(tree);
  rbtree_free_data(tree);
  rbtree_free(tree);
}

int rbtree_find_node(RBTree *tree, void *key, RBNode **res_node) {
  RBNode *tmp_node = tree->root, *new_node;
  if (tmp_node == NULL)
    return 2;
  int cmp_res = tree->cmp(key, tmp_node->key);
  while (cmp_res != 0) {
    if (cmp_res > 0)
      new_node = tmp_node->right;
    else if (cmp_res < 0)
      new_node = tmp_node->left;
    if (new_node == NULL)
      break;
    tmp_node = new_node;
    cmp_res = tree->cmp(key, tmp_node->key);
  }
  if (res_node != NULL)  // still set closest
    *res_node = tmp_node;  // still set closest
  if (cmp_res != 0)
    return 1;  // key not found
  return 0;
}

void * rbtree_get_data(RBNode *node) {
  return node->data;
}

void rbtree_set_data(RBNode *node, void *data) {
  node->data = data;
}

int rbtree_find(RBTree *tree, void *key, void *data) {
  RBNode *node;
  int ret_val = rbtree_find_node(tree, key, &node);
  if (ret_val != 0)
    return ret_val;
  *((int **)data) = rbtree_get_data(node);
  return 0;
}

void rbtree_rotate_left(RBNode *node) {
  RBNode *old_left = node->left;
  void *old_key = node->key;
  void *old_data = node->data;
  int old_color = node->color;
  node->left = node->right;
  if (node->left != NULL)
    node->left->parent = node;
  node->key = node->right->key;
  node->data = node->right->data;
  node->color = node->right->color;
  node->right = node->left->right;
  if (node->right != NULL)
    node->right->parent = node;
  node->left->right = node->left->left;
  if (node->left->right != NULL)
    node->left->right->parent = node->left;
  node->left->left = old_left;
  if (node->left->left != NULL)
    node->left->left->parent = node->left;
  node->left->key = old_key;
  node->left->data = old_data;
  node->left->color = old_color;
}

void rbtree_rotate_right(RBNode *node) {
  RBNode *old_right = node->right;
  void *old_key = node->key;
  void *old_data = node->data;
  int old_color = node->color;
  node->right = node->left;
  if (node->right != NULL)
    node->right->parent = node;
  node->key = node->left->key;
  node->data = node->left->data;
  node->color = node->left->color;
  node->left = node->right->left;
  if (node->left != NULL)
    node->left->parent = node;
  node->right->left = node->right->right;
  if (node->right->left != NULL)
    node->right->left->parent = node->right;
  node->right->right = old_right;
  if (node->right->right != NULL)
    node->right->right->parent = node->right;
  node->right->key = old_key;
  node->right->data = old_data;
  node->right->color = old_color;
}

RBNode * rbtree_grandparent(RBNode *node) {
  if (node->parent != NULL)
    return node->parent->parent;
  return NULL;
}

RBNode * rbtree_uncle(RBNode *node) {
  RBNode *grandparent = rbtree_grandparent(node);
  if (grandparent != NULL) {
    printf("right: %d\n", grandparent->right);
    printf("left: %d\n", grandparent->left);
    if (node->parent == grandparent->left)
      return grandparent->right;
    return grandparent->left;
  }
  return NULL;
}

RBNode * rbtree_sibling(RBNode *node) {
  if (node->parent != NULL) {
    if (node == node->parent->left)
      return node->parent->right;
    return node->parent->left;
  }
  return NULL;
}

int rbtree_color(RBNode *node) {
  if (node == NULL)
    return 0;
  return node->color;
}

void rbtree_insert_case5(RBNode *node) {
  printf("Case 5\n");
  RBNode *grandparent = rbtree_grandparent(node);
  node->parent->color = 0;
  grandparent->color = 1;
  if (node == node->parent->left)
    rbtree_rotate_right(grandparent);
  else
    rbtree_rotate_left(grandparent);
}

void rbtree_insert_case4(RBNode *node) {
  printf("Case 4\n");
  RBNode *grandparent = rbtree_grandparent(node);
  RBNode *next = node;
  if (node == node->parent->right && node->parent == grandparent->left) {
    rbtree_rotate_left(node->parent);
    next = node->left;
  } else if (node == node->parent->left && node->parent == grandparent->right) {
    rbtree_rotate_right(node->parent);
    next = node->left;
  }
  rbtree_insert_case5(next);
}

void rbtree_insert_case3(RBNode *node) {
  printf("Case 3\n");
  RBNode *uncle = rbtree_uncle(node);
  printf("uncle: %d\n", uncle);
  if (uncle != NULL && uncle->color == 1) {
    node->parent->color = 0;
    uncle->color = 0;
    rbtree_grandparent(node)->color = 1;
    rbtree_insert_case1(rbtree_grandparent(node));
    return;
  }
  rbtree_insert_case4(node);
}

void rbtree_insert_case2(RBNode *node) {
  printf("Case 2\n");
  if (node->parent->color == 0)
    return;
  rbtree_insert_case3(node);
}

void rbtree_insert_case1(RBNode *node) {
  printf("Case 1\n");
  if (node->parent == NULL && node->color == 1)  // root is red
    node->color = 0;
  else
    rbtree_insert_case2(node);
}

int rbtree_insert_node_at(RBTree *tree, RBNode *node, RBNode *old_node) {
  int cmp_val = tree->cmp(node->key, old_node->key);
  if (cmp_val > 0)
    old_node->right = node;
  else if (cmp_val < 0)
    old_node->left = node;
  else  // duplicate key
    return 1;
  rbtree_insert_case1(node);
  return 0;
}

int rbtree_insert_node(RBTree *tree, RBNode *node) {
  RBNode *old_node;
  if (tree->root == NULL) {
    tree->root = node;
    rbtree_insert_case1(node);
    return 0;
  }
  rbtree_find_node(tree, node->key, &old_node);
  node->parent = old_node;
  if (tree->root != NULL) {
    printf("parent: %s\n", string_str(node->parent->key));
    if (node->parent->parent != NULL)
      printf("grandparent: %s\n", string_str(node->parent->parent->key));
    printf("root: %s\n", string_str(tree->root->key));
    if (tree->root->left != NULL)
      printf("root->left: %s\n", string_str(tree->root->left->key));
    if (tree->root->right != NULL)
      printf("root->right: %s\n", string_str(tree->root->right->key));
    if (rbtree_grandparent(node) != NULL)
      printf("grandparent: %s\n", string_str(rbtree_grandparent(node)->key));
  }
  return rbtree_insert_node_at(tree, node, old_node);
}

int rbtree_insert(RBTree *tree, void *key, void *data) {
  RBNode *node = rbtree_alloc_node();
  node->key = key;
  rbtree_set_data(node, data);
  int ret_val = rbtree_insert_node(tree, node);
  if (ret_val == 1)
    rbtree_free_node(node);
  if (tree->root != NULL) {
    printf("root: %s\n", string_str(tree->root->key));
    if (tree->root->left != NULL) {
      printf("left: %s\n", string_str(tree->root->left->key));
      printf("left color: %d\n", tree->root->left->color);
    }
    if (tree->root->right != NULL) {
      printf("right: %s\n", string_str(tree->root->right->key));
      printf("right color: %d\n", tree->root->right->color);
    }
  }
  return ret_val;
}

int rbtree_update_node(RBTree *tree, RBNode *node, void *data, void *old_data) {
  if (old_data != NULL)
    *((int **)(old_data)) = rbtree_get_data(node);
  rbtree_set_data(node, data);
  return 0;
}

int rbtree_update(RBTree *tree, void *key, void *data, void *old_data) {
  RBNode *node;
  int ret_val = rbtree_find_node(tree, key, &node);
  if (ret_val != 0)
    return ret_val;
  return rbtree_update_node(tree, node, data, old_data);
}

RBNode * rbtree_min_node(RBNode *node) {
  if (node->left == NULL)
    return node;
  return rbtree_min_node(node->left);
}

void rbtree_remove_case6(RBNode *node) {
  RBNode *sibling = rbtree_sibling(node);
  sibling->color = node->parent->color;
  if (node == node->parent->left) {
    sibling->right->color = 0;
    rbtree_rotate_left(node->parent);
  } else {
    sibling->left->color = 0;
    rbtree_rotate_right(node->parent);
  }
}

void rbtree_remove_case5(RBNode *node) {
  RBNode *sibling = rbtree_sibling(node);
  if (node == node->parent->left &&
      rbtree_color(sibling->left) == 1 && rbtree_color(sibling->right) == 0) {
    sibling->color = 1;
    sibling->left->color = 0;
    rbtree_rotate_right(sibling);
  } else if (node == node->parent->right &&
             rbtree_color(sibling->left) == 0 && rbtree_color(sibling->right) == 1) {
    sibling->color = 1;
    sibling->right->color =0;
    rbtree_rotate_left(sibling);
  }
  rbtree_remove_case6(node);
}

void rbtree_remove_case4(RBNode *node) {
  RBNode *sibling = rbtree_sibling(node);
  if (node->parent->color == 1 && sibling->color == 0 &&
      rbtree_color(sibling->left) == 0 && rbtree_color(sibling->right) == 0) {
    sibling->color = 1;
    node->parent->color = 0;
  } else
    rbtree_remove_case5(node);
}

void rbtree_remove_case3(RBNode *node) {
  RBNode *sibling = rbtree_sibling(node);
  if (node->parent->color == 0 && sibling->color == 0 &&
      rbtree_color(sibling->left) == 0 && rbtree_color(sibling->right) == 0) {
    sibling->color = 1;
    rbtree_remove_case1(node->parent);
  } else
    rbtree_remove_case4(node);
}

void rbtree_remove_case2(RBNode *node) {
  RBNode *sibling = rbtree_sibling(node);
  if (sibling->color == 1) {
    sibling->color = 0;
    node->parent->color = 1;
    if (node == node->parent->left)
      rbtree_rotate_left(node->parent);
    else
      rbtree_rotate_right(node->parent);
  }
  rbtree_remove_case3(node);
}


void rbtree_remove_case1(RBNode *node) {
  if (node->parent == NULL)
    return;
  rbtree_remove_case2(node);
}

void * rbtree_remove_node_at(RBTree *tree, RBNode *node, void *old_key) {
  if (tree->root != NULL) {
    printf("root: %s\n", string_str(tree->root->key));
    if (tree->root->left != NULL) {
      printf("left: %s\n", string_str(tree->root->left->key));
      printf("left color: %d\n", tree->root->left->color);
    }
    if (tree->root->right != NULL) {
      printf("right: %s\n", string_str(tree->root->right->key));
      printf("right color: %d\n", tree->root->right->color);
    }
  }
  void *data = rbtree_get_data(node);
  if (old_key != NULL)
    *((int **)old_key) = node->key;
  if (node->left == NULL && node->right == NULL) { // no children
    /* node->color = 0; */
    if (node->color == 0)
      rbtree_remove_case1(node);
    if (node == node->parent->left)
      node->parent->left = NULL;
    else
      node->parent->right = NULL;
    rbtree_free_node(node);
  } else if ((node->left == NULL) != (node->right == NULL)) {  // one child
    printf("One child!\n");
    RBNode *child;
    if (node->left != NULL)  // one child
      child = node->left;
    else
      child = node->right;
    if (node->parent == NULL)
      tree->root = child;
    else {
      if (node == node->parent->left)
        node->parent->left = child;
      else
        node->parent->right = child;
    }
    child->parent = node->parent;
    child->color = 0;  // has to have been red
    rbtree_free_node(node);
  } else {  // two children
    RBNode *min = rbtree_min_node(node->right);
    node->key = min->key;
    node->data = min->data;
    rbtree_remove_node_at(tree, min, NULL);
  }
  return data;
}

int rbtree_is_leaf(RBNode *node) {
  if (node->left == NULL && node->right == NULL)
    return 1;
  return 0;
}

int rbtree_remove(RBTree *tree, void *key, void *data, void *old_key) {
  RBNode *node;
  int ret_val = rbtree_find_node(tree, key, &node);
  if (ret_val != 0)
    return ret_val;
  if (rbtree_is_leaf(tree->root)) {
    *((int **)data) = rbtree_get_data(tree->root);
    *((int **)old_key) = tree->root->key;
    rbtree_free_node(tree->root);
    tree->root = NULL;
    return 0;
  }
  *((int **)data) = rbtree_remove_node_at(tree, node, old_key);
  return 0;
}

int rbtree_remove_node(RBTree *tree, RBNode *node, void *data) {
  return rbtree_remove(tree, node->key, data, NULL);
}

int rbtree_node_count_nodes(RBNode *node) {
  if (node == NULL)
    return 0;
  return 1 + rbtree_node_count_nodes(node->left) + rbtree_node_count_nodes(node->right);
}

int rbtree_count_nodes(RBTree *tree) {
  return rbtree_node_count_nodes(tree->root);
}

void rbtree_node_in_order(RBTree *tree, RBNode *node, List *res) {
  if (node != NULL) {
    rbtree_node_in_order(tree, node->left, res);
    KeyVal *kv = keyval_alloc(node->key, rbtree_get_data(node),
                              tree->key_to_string, tree->data_to_string,
                              tree->free_key_string, tree->free_data_string,
                              tree->cmp);
    list_append(res, kv);
    rbtree_node_in_order(tree, node->right, res);
  }
}

List * rbtree_in_order(RBTree *tree) {
  List *res = list_alloc(keyval_to_string_void, string_free_str, keyval_free_void,
                         keyval_cmp);
  rbtree_node_in_order(tree, tree->root, res);
  return res;
}
