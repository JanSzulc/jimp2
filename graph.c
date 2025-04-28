#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graph* load_graph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    int max_row_nodes;
    fscanf(file, "%d", &max_row_nodes);

    // Czytanie indeksów węzłów (pomijamy je)
    int value;
    for (int i = 0; i < max_row_nodes; i++) {
        fscanf(file, "%d ;", &value);
    }

    // Czytanie wskaźników (pomijamy je)
    for (int i = 0; i < max_row_nodes; i++) {
        fscanf(file, "%d ;", &value);
    }

    // Liczymy liczbę węzłów:
    int node_count = max_row_nodes; // uproszczone na podstawie danych

    Graph *graph = malloc(sizeof(Graph));
    graph->num_nodes = node_count;
    graph->adjacency_matrix = malloc(node_count * sizeof(int*));
    for (int i = 0; i < node_count; i++) {
        graph->adjacency_matrix[i] = calloc(node_count, sizeof(int));
    }

    // Czytanie krawędzi
    int src, dst;
    while (fscanf(file, "%d - %d", &src, &dst) == 2) {
        if (src < node_count && dst < node_count) {
            graph->adjacency_matrix[src][dst] = 1;
            graph->adjacency_matrix[dst][src] = 1; // Graf nieskierowany
        }
    }

    fclose(file);
    return graph;
}

void free_graph(Graph *graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        free(graph->adjacency_matrix[i]);
    }
    free(graph->adjacency_matrix);
    free(graph);
}
