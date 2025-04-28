#include <stdlib.h>
#include <math.h>
#include "partition.h"

// Prosty algorytm losowego przydziału (na początek)
int* partition_graph(Graph *graph, int parts, int margin) {
    int *partition = malloc(graph->num_nodes * sizeof(int));
    int *count = calloc(parts, sizeof(int));

    int avg_nodes = graph->num_nodes / parts;
    int max_nodes = avg_nodes + (avg_nodes * margin / 100);

    for (int i = 0; i < graph->num_nodes; i++) {
        int best_part = 0;
        int min_load = count[0];

        for (int p = 1; p < parts; p++) {
            if (count[p] < min_load) {
                best_part = p;
                min_load = count[p];
            }
        }

        partition[i] = best_part;
        count[best_part]++;
    }

    free(count);
    return partition;
}
