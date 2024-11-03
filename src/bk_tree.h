#ifndef BK_TREE_H
#define BK_TREE_H

#include <stdlib.h>

typedef struct BKTreeNode {
    char *word;
    int child_count;
    int *distances;
    struct BKTreeNode **children;
} BKTreeNode;

typedef struct {
    BKTreeNode *root;
    int (*distance_func)(const char *, const char *);
} BKTree;

BKTree *bk_tree_create(int (*distance_func)(const char *, const char *));
void bk_tree_insert(BKTree *tree, const char *word);
void bk_tree_search(BKTree *tree, const char *query, int max_distance, char ***results, int *result_count);
void bk_tree_free(BKTree *tree);

#endif 