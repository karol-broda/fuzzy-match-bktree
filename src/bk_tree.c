#include "bk_tree.h"
#include <string.h>

static BKTreeNode *bk_tree_node_create(const char *word) {
    BKTreeNode *node = (BKTreeNode *)malloc(sizeof(BKTreeNode));
    node->word = strdup(word);
    node->child_count = 0;
    node->distances = NULL;
    node->children = NULL;
    return node;
}

static void bk_tree_node_free(BKTreeNode *node) {
    if (node) {
        free(node->word);
        for (int i = 0; i < node->child_count; i++) {
            bk_tree_node_free(node->children[i]);
        }
        free(node->distances);
        free(node->children);
        free(node);
    }
}

BKTree *bk_tree_create(int (*distance_func)(const char *, const char *)) {
    BKTree *tree = (BKTree *)malloc(sizeof(BKTree));
    tree->root = NULL;
    tree->distance_func = distance_func;
    return tree;
}

void bk_tree_insert(BKTree *tree, const char *word) {
    if (!tree->root) {
        tree->root = bk_tree_node_create(word);
        return;
    }

    BKTreeNode *node = tree->root;
    int distance = tree->distance_func(node->word, word);

    while (1) {
        int found = 0;
        for (int i = 0; i < node->child_count; i++) {
            if (node->distances[i] == distance) {
                node = node->children[i];
                distance = tree->distance_func(node->word, word);
                found = 1;
                break;
            }
        }

        if (!found) {
            node->child_count++;
            node->distances = realloc(node->distances, node->child_count * sizeof(int));
            node->children = realloc(node->children, node->child_count * sizeof(BKTreeNode *));
            node->distances[node->child_count - 1] = distance;
            node->children[node->child_count - 1] = bk_tree_node_create(word);
            break;
        }
    }
}

static void bk_tree_search_recursive(BKTreeNode *node, const char *query, int max_distance, int (*distance_func)(const char *, const char *), char ***results, int *result_count, int *result_capacity) {
    int distance = distance_func(node->word, query);

    if (distance <= max_distance) {
        if (*result_count >= *result_capacity) {
            *result_capacity *= 2;
            *results = realloc(*results, (*result_capacity) * sizeof(char *));
        }
        (*results)[*result_count] = strdup(node->word);
        (*result_count)++;
    }

    int start = distance - max_distance;
    if (start < 0) start = 0;
    int end = distance + max_distance;

    for (int i = 0; i < node->child_count; i++) {
        if (node->distances[i] >= start && node->distances[i] <= end) {
            bk_tree_search_recursive(node->children[i], query, max_distance, distance_func, results, result_count, result_capacity);
        }
    }
}

void bk_tree_search(BKTree *tree, const char *query, int max_distance, char ***results, int *result_count) {
    *result_count = 0;
    int result_capacity = 10;
    *results = malloc(result_capacity * sizeof(char *));

    if (tree->root) {
        bk_tree_search_recursive(tree->root, query, max_distance, tree->distance_func, results, result_count, &result_capacity);
    }
}

void bk_tree_free(BKTree *tree) {
    if (tree) {
        bk_tree_node_free(tree->root);
        free(tree);
    }
}
