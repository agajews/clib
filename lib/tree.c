#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "str.h"
#include "list.h"
#include "keyval.h"

void tree_init(Tree *tree, int (*cmp)(const void *, const void *),
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

Tree * tree_alloc(int (*cmp)(const void *, const void *),
                  String * (*key_to_string)(void *), String * (*data_to_string)(void *),
                  void (*free_key_string)(String *), void (*free_data_string)(String *),
                  void (*free_key)(void *), void (*free_data)(void *)) {
  Tree *tree = malloc(sizeof(Tree));
  tree_init(tree, cmp, key_to_string, data_to_string, free_key_string, free_data_string,
            free_key, free_data);
  return tree;
}

Node * tree_alloc_node() {
  return malloc(sizeof(Node));
}

void tree_free_node(Node *node) {
  if (node == NULL)
    return;
  free(node);
}

void tree_free_node_key(Tree *tree, Node *node) {
  if (node == NULL)
    return;
  if (tree_is_leaf(node)) {
    tree->free_key(node->key);
  }
}

void tree_free_node_data(Tree *tree, Node *node) {
  if (node == NULL)
    return;
  if (tree_is_leaf(node)) {
    tree->free_data(tree_get_data(node));
  }
}

void tree_free_nodes(Node *node) {
  if (node == NULL)
    return;
  if (!tree_is_leaf(node)) {
    tree_free_nodes(node->left);
    tree_free_nodes(node->right);
  }
  tree_free_node(node);
}

void tree_free_nodes_keys(Tree *tree, Node *node) {
  if (node == NULL)
    return;
  if (!tree_is_leaf(node)) {
    tree_free_nodes_keys(tree, node->left);
    tree_free_nodes_keys(tree, node->right);
  }
  tree_free_node_key(tree, node);
}

void tree_free_nodes_data(Tree *tree, Node *node) {
  if (node == NULL)
    return;
  if (!tree_is_leaf(node)) {
    tree_free_nodes_data(tree, node->left);
    tree_free_nodes_data(tree, node->right);
  }
  tree_free_node_data(tree, node);
}

void tree_free(Tree *tree) {
  tree_free_nodes(tree->root);
  free(tree);
}

void tree_free_keys(Tree *tree) {
  tree_free_nodes_keys(tree, tree->root);
}

void tree_free_data(Tree *tree) {
  tree_free_nodes_data(tree, tree->root);
}

void tree_free_all(Tree *tree) {
  tree_free_keys(tree);
  tree_free_data(tree);
  tree_free(tree);
}

void tree_set_leaf(Node *node) {
  node->right = NULL;
}

int tree_is_leaf(Node *node) {
  if (node->right == NULL)
    return 1;
  return 0;
}

int tree_find_node_parent(Tree *tree, void *key, Node **res_node, Node **res_parent) {
  Node *tmp_node = tree->root;
  Node *parent_node = NULL;
  if (tmp_node == NULL)
    return 2;
  int cmp_res = tree->cmp(key, tmp_node->key);
  while (!tree_is_leaf(tmp_node)) {
    parent_node = tmp_node;
    if (cmp_res >= 0)
      tmp_node = tmp_node->right;
    else if (cmp_res < 0)
      tmp_node = tmp_node->left;
    cmp_res = tree->cmp(key, tmp_node->key);
  }
  if (res_node != NULL)  // still set closest
    *res_node = tmp_node;  // still set closest
  if (res_parent != NULL)
    *res_parent = parent_node;
  if (cmp_res != 0)
    return 1;  // key not found
  return 0;
}

int tree_find_node(Tree *tree, void *key, Node **res_node) {
  return tree_find_node_parent(tree, key, res_node, NULL);
}

void * tree_get_data(Node *leaf) {
  return leaf->left;
}

void tree_set_data(Node *leaf, void *data) {
  leaf->left = data;
}

int tree_find(Tree *tree, void *key, void *data) {
  Node *node;
  int ret_val = tree_find_node(tree, key, &node);
  if (ret_val != 0)
    return ret_val;
  *((int **)data) = tree_get_data(node);
  return 0;
}

int tree_insert_node_at(Tree *tree, Node *node, Node *old_node) {
  Node *tmp_node = tree_alloc_node();
  tmp_node->key = old_node->key;
  tree_set_leaf(tmp_node);
  tree_set_data(tmp_node, tree_get_data(old_node));
  int cmp_val = tree->cmp(node->key, old_node->key);
  if (cmp_val > 0) {
    old_node->key = node->key;
    old_node->left = tmp_node;
    old_node->right = node;
  } else if (cmp_val < 0) {
    old_node->left = node;
    old_node->right = tmp_node;
  } else
    return 1;  // duplicate key
  return 0;
}

int tree_insert_node(Tree *tree, Node *node) {
  Node *old_node;
  int ret_val = tree_find_node(tree, node->key, &old_node);
  if (ret_val == 2) {
    tree->root = node;
    return 0;
  }
  else
    return tree_insert_node_at(tree, node, old_node);
}

int tree_insert(Tree *tree, void *key, void *data) {
  Node *node = tree_alloc_node();
  node->key = key;
  tree_set_leaf(node);
  tree_set_data(node, data);
  return tree_insert_node(tree, node);
}

int tree_update_node(Tree *tree, Node *node, void *data, void *old_data) {
  if (!tree_is_leaf(node))
    return 1;
  if (old_data != NULL)
    *((int **)(old_data)) = tree_get_data(node);
  tree_set_data(node, data);
  return 0;
}

int tree_update(Tree *tree, void *key, void *data, void *old_data) {
  Node *node;
  int ret_val = tree_find_node(tree, key, &node);
  if (ret_val != 0)
    return ret_val;
  return tree_update_node(tree, node, data, old_data);
}

void * tree_remove_node_at(Tree *tree, Node *node, Node *parent, void *old_key) {
  Node *sibling;
  if (node == parent->right) {
    sibling = parent->left;
    parent->key = sibling->key;
  } else
    sibling = parent->right;
  parent->left = tree_get_data(sibling);
  parent->right = NULL;
  void *data = tree_get_data(node);
  if (old_key != NULL)
    *((int **)old_key) = node->key;
  tree_free_node(node);
  tree_free_node(sibling);
  return data;
}

int tree_remove(Tree *tree, void *key, void *data, void *old_key) {
  Node *parent, *node;
  int ret_val = tree_find_node_parent(tree, key, &node, &parent);
  if (ret_val != 0)
    return ret_val;
  if (tree_is_leaf(tree->root)) {
    *((int **)data) = tree_get_data(tree->root);
    *((int **)old_key) = tree->root->key;
    tree_free_node(tree->root);
    tree->root = NULL;
    return 0;
  }
  *((int **)data) = tree_remove_node_at(tree, node, parent, old_key);
  return 0;
}

int tree_remove_node(Tree *tree, Node *node, void *data) {
  return tree_remove(tree, node->key, data, NULL);
}

int tree_node_count_nodes(Node *node) {
  if (tree_is_leaf(node))
    return 1;
  return tree_node_count_nodes(node->left) + tree_node_count_nodes(node->right);
}

int tree_count_nodes(Tree *tree) {
  if (tree->root == NULL)
    return 0;
  return tree_node_count_nodes(tree->root);
}

void tree_node_in_order(Tree *tree, Node *node, List *res) {
  if (tree_is_leaf(node)) {
    KeyVal *kv = keyval_alloc(node->key, tree_get_data(node),
                              tree->key_to_string, tree->data_to_string,
                              tree->free_key_string, tree->free_data_string);
    list_append(res, kv);
    return;
  }
  tree_node_in_order(tree, node->left, res);
  tree_node_in_order(tree, node->right, res);
}

List * tree_in_order(Tree *tree) {
  List *res = list_alloc(keyval_to_string_void, string_free_str, keyval_free_void);
  if (tree->root != NULL)
    tree_node_in_order(tree, tree->root, res);
  return res;
}
