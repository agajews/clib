#ifndef _TREE_H_
#define _TREE_H_

typedef struct List_ List;

typedef struct String_ String;

typedef struct RBNode_ {
  struct RBNode_ *parent;
  struct RBNode_ *left;
  struct RBNode_ *right;
  void *key;
  void *data;
  int color;
} RBNode;

typedef struct RBTree_ {
  struct RBNode_ *root;
  int (*cmp)(const void *key_a, const void *key_b);
  String * (*key_to_string)(void *key);
  String * (*data_to_string)(void *data);
  void (*free_key_string)(String *s);
  void (*free_data_string)(String *s);
  void (*free_key)(void *key);
  void (*free_data)(void *data);
} RBTree;

void rbtree_init(RBTree *tree, int (*cmp)(const void *, const void *),
                 String * (*key_to_string)(void *), String * (*data_to_string)(void *),
                 void (*free_key_string)(String *), void (*free_data_string)(String *),
                 void (*free_key)(void *), void (*free_data)(void *));

RBTree * rbtree_alloc(int (*cmp)(const void *, const void *),
                      String * (*key_to_string)(void *), String * (*data_to_string)(void *),
                      void (*free_key_string)(String *), void (*free_data_string)(String *),
                      void (*free_key)(void *), void (*free_data)(void *));

void rbtree_init_node(RBNode *node);

RBNode * rbtree_alloc_node();

void rbtree_free_node(RBNode *node);

void rbtree_free_node_key(RBTree *tree, RBNode *node);

void rbtree_free_node_data(RBTree *tree, RBNode *node);

void rbtree_free_nodes(RBNode *node);

void rbtree_free_nodes_keys(RBTree *tree, RBNode *node);

void rbtree_free_nodes_data(RBTree *tree, RBNode *node);

void rbtree_free(RBTree *tree);

void rbtree_free_keys(RBTree *tree);

void rbtree_free_data(RBTree *tree);

void rbtree_free_all(RBTree *tree);

int rbtree_find_node(RBTree *tree, void *key, RBNode **res_node);

void * rbtree_get_data(RBNode *node);

void rbtree_set_data(RBNode *node, void *data);

int rbtree_find(RBTree *tree, void *key, void *data);

void rbtree_rotate_left(RBNode *node);

void rbtree_rotate_right(RBNode *node);

RBNode * rbtree_grandparent(RBNode *node);

RBNode * rbtree_uncle(RBNode *node);

RBNode * rbtree_sibling(RBNode *node);

int rbtree_color(RBNode *node);

void rbtree_insert_case5(RBNode *node);

void rbtree_insert_case4(RBNode *node);

void rbtree_insert_case3(RBNode *node);

void rbtree_insert_case2(RBNode *node);

void rbtree_insert_case1(RBNode *node);

int rbtree_insert_node_at(RBTree *tree, RBNode *node, RBNode *old_node);

int rbtree_insert_node(RBTree *tree, RBNode *node);

int rbtree_insert(RBTree *tree, void *key, void *data);

int rbtree_update_node(RBTree *tree, RBNode *node, void *data, void *old_data);

int rbtree_update(RBTree *tree, void *key, void *data, void *old_data);

RBNode * rbtree_min_node(RBNode *node);

void rbtree_remove_case6(RBNode *node);

void rbtree_remove_case5(RBNode *node);

void rbtree_remove_case4(RBNode *node);

void rbtree_remove_case3(RBNode *node);

void rbtree_remove_case2(RBNode *node);

void rbtree_remove_case1(RBNode *node);

void * rbtree_remove_node_at(RBTree *tree, RBNode *node, void *old_key);

int rbtree_is_leaf(RBNode *node);

int rbtree_remove(RBTree *tree, void *key, void *data, void *old_key);

int rbtree_remove_node(RBTree *tree, RBNode *node, void *data);

int rbtree_node_count_nodes(RBNode *node);

int rbtree_count_nodes(RBTree *tree);

void rbtree_node_in_order(RBTree *tree, RBNode *node, List *res);

List * rbtree_in_order(RBTree *tree);

#endif
