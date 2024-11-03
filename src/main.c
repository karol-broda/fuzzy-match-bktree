#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bk_tree.h"
#include "levenshtein.h"

#define MAX_WORD_LENGTH 100

void load_words(const char *filename, BKTree *tree) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "failed to open %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_WORD_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        bk_tree_insert(tree, buffer);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    const char *wordlist_filename = "words.txt";
    const char *query = "house";
    int max_distance = 2;

    if (argc > 1) query = argv[1];
    if (argc > 2) max_distance = atoi(argv[2]);

    BKTree *tree = bk_tree_create(levenshtein_distance);
    printf("loading words from %s...\n", wordlist_filename);
    load_words(wordlist_filename, tree);
    printf("finished loading words :)\n");

    char **results;
    int result_count;

    printf("searching for words within distance %d of '%s'...\n", max_distance, query);
    bk_tree_search(tree, query, max_distance, &results, &result_count);

    printf("found %d words:\n", result_count);
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", results[i]);
        free(results[i]);
    }
    free(results);

    bk_tree_free(tree);
    return 0;
}
