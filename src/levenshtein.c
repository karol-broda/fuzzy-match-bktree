#include "levenshtein.h"
#include <string.h>
#include <stdlib.h>

int levenshtein_distance(const char *s1, const char *s2) {
    int len_s1 = strlen(s1);
    int len_s2 = strlen(s2);

    int *v0 = (int *)malloc((len_s2 + 1) * sizeof(int));
    int *v1 = (int *)malloc((len_s2 + 1) * sizeof(int));

    for (int i = 0; i <= len_s2; i++)
        v0[i] = i;

    for (int i = 0; i < len_s1; i++) {
        v1[0] = i + 1;

        for (int j = 0; j < len_s2; j++) {
            int cost = (s1[i] == s2[j]) ? 0 : 1;
            int deletion = v0[j + 1] + 1;
            int insertion = v1[j] + 1;
            int substitution = v0[j] + cost;

            v1[j + 1] = deletion < insertion ? (deletion < substitution ? deletion : substitution) : (insertion < substitution ? insertion : substitution);
        }

        int *temp = v0;
        v0 = v1;
        v1 = temp;
    }

    int distance = v0[len_s2];
    free(v0);
    free(v1);
    return distance;
}
