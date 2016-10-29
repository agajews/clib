#ifndef _TREE_H_
#define _TREE_H_

typedef struct List_ List;

typedef struct String_ String;

typedef struct Node_ {
  struct Node_ *left;  // data when leaf
  struct Node_ *right;  // NULL when leaf
  void *key;
} Node;

typedef struct Tree_ {
  struct Node_ *root;
  int (*cmp)(const void *key_a, const void *key_b);
  String * (*key_to_string)(void *key);
  String * (*data_to_string)(void *data);
  void (*free_key_string)(String *s);
  void (*free_data_string)(String *s);
  void (*free_key)(void *key);
  void (*free_data)(void *data);
} Tree;

void tree_init(Tree *tree, int (*cmp)(const void *, const void *),
               String * (*key_to_string)(void *), String * (*data_to_string)(void *),
               void (*free_key_string)(String *), void (*free_data_string)(String *),
               void (*free_key)(void *), void (*free_data)(void *));

Tree * tree_alloc(int (*cmp)(const void *, const void *),
                  String * (*key_to_string)(void *), String * (*data_to_string)(void *),
                  void (*free_key_string)(String *), void (*free_data_string)(String *),
                  void (*free_key)(void *), void (*free_data)(void *));

Node * tree_alloc_node();

void tree_free_node(Node *node);

void tree_free_node_key(Tree *tree, Node *node);

void tree_free_node_data(Tree *tree, Node *node);

void tree_free_nodes(Node *node);

void tree_free_nodes_keys(Tree *tree, Node *node);

void tree_free_nodes_data(Tree *tree, Node *node);

void tree_free(Tree *tree);

void tree_free_keys(Tree *tree);

void tree_free_data(Tree *tree);

void tree_free_all(Tree *tree);

void tree_set_leaf(Node *node);

int tree_is_leaf(Node *node);

int tree_find_node_parent(Tree *tree, void *key, Node **res_node, Node **res_parent);

int tree_find_node(Tree *tree, void *key, Node **res_node);

void * tree_get_data(Node *leaf);

void tree_set_data(Node *leaf, void *data);

int tree_find(Tree *tree, void *key, void *data);

int tree_insert_node_at(Tree *tree, Node *node, Node *old_node);

int tree_insert_node(Tree *tree, Node *node);

int tree_insert(Tree *tree, void *key, void *data);

int tree_update_node(Tree *tree, Node *node, void *data, void *oldData);

int tree_update(Tree *tree, void *key, void *data, void *oldData);

void * tree_remove_node_at(Tree *tree, Node *node, Node *parent, void *old_key);

int tree_remove(Tree *tree, void *key, void *data, void *old_key);

int tree_remove_node(Tree *tree, Node *node, void *data);

int tree_node_count_nodes(Node *node);

int tree_count_nodes(Tree *tree);

void tree_node_in_order(Tree *tree, Node *node, List *res);

List * tree_in_order(Tree *tree);

#endif
