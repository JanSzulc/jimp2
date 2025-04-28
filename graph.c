#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graph* load_graph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    int max_row_nodes;
    fscanf(file, "%d", &max_row_nodes);

    int id_list[10000];
    int row_ptrs[10000];
    int neighbors[100000];
    int id_to_index[10000];
    for (int i = 0; i < 10000; i++) id_to_index[i] = -1;

    int id_count = 0;
    int row_ptr_count = 0;
    int neighbors_count = 0;

    while (1) {
        int value;
        if (fscanf(file, "%d;", &value) != 1) break;
        id_list[id_count++] = value;
    }

    for (int i = 0; i < id_count; i++) {
        id_to_index[id_list[i]] = i;
    }

    for (int i = 0; i < id_count; i++) {
        int value;
        fscanf(file, "%d;", &value);
        row_ptrs[row_ptr_count++] = value;
    }

    while (1) {
        int value;
        if (fscanf(file, "%d;", &value) != 1) break;
        neighbors[neighbors_count++] = value;
    }

    Graph *graph = malloc(sizeof(Graph));
    graph->num_nodes = id_count;
    graph->id_list = malloc(id_count * sizeof(int));
    graph->adjacency_matrix = malloc(id_count * sizeof(int*));

    for (int i = 0; i < id_count; i++) {
        graph->id_list[i] = id_list[i];
        graph->adjacency_matrix[i] = calloc(id_count, sizeof(int));
    }

    for (int i = 0; i < id_count; i++) {
        int start = row_ptrs[i];
        int end = (i + 1 < row_ptr_count) ? row_ptrs[i + 1] : neighbors_count;

        for (int j = start; j < end; j++) {
            int neighbor_id = neighbors[j];
            if (id_to_index[neighbor_id] != -1) {
                int neighbor_idx = id_to_index[neighbor_id];
                graph->adjacency_matrix[i][neighbor_idx] = 1;
                graph->adjacency_matrix[neighbor_idx][i] = 1;
            }
        }
    }

    fclose(file);
    return graph;
}
void save_graph_as_txt(const char *filename, Graph *graph) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Błąd: Nie można otworzyć pliku do zapisu grafu.\n");
        return;
    }

    fprintf(file, "# Wierzcholki (tylko ID)\n");
    for (int i = 0; i < graph->num_nodes; i++) {
        fprintf(file, "%d\n", graph->id_list[i]);
    }

    fprintf(file, "\n# Krawedzie (ID1 ID2)\n");
    for (int i = 0; i < graph->num_nodes; i++) {
        for (int j = i + 1; j < graph->num_nodes; j++) {
            if (graph->adjacency_matrix[i][j]) {
                fprintf(file, "%d %d\n", graph->id_list[i], graph->id_list[j]);
            }
        }
    }

    fclose(file);
}


void free_graph(Graph *graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        free(graph->adjacency_matrix[i]);
    }
    free(graph->adjacency_matrix);
    free(graph->id_list);
    free(graph);
}