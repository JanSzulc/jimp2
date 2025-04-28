#include <stdlib.h>
#include <string.h>
#include "partition.h"

int* partition_graph(Graph *graph, int parts, int margin) {
    int *partition = malloc(graph->num_nodes * sizeof(int));
    int *count = calloc(parts, sizeof(int));

    int avg = graph->num_nodes / parts;
    int allowed_diff = avg * margin / 100;

    for (int i = 0; i < graph->num_nodes; i++) {
        int best_part = 0;
        int min_count = count[0];

        for (int p = 1; p < parts; p++) {
            if (count[p] < min_count) {
                best_part = p;
                min_count = count[p];
            }
        }

        partition[i] = best_part;
        count[best_part]++;
    }

    free(count);
    return partition;
}