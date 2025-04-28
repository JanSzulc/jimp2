#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void print_error(const char *message, int exit_code) {
    fprintf(stderr, "%s\n", message);
    exit(exit_code);
}

void zapisz_wyniki(const char *filename, int *partition, Graph *graph, int binary) {
    FILE *file = fopen(filename, binary ? "wb" : "w");
    if (!file) print_error("Nie mozna otworzyc pliku do zapisu", 3);

    if (binary) {
        fwrite(partition, sizeof(int), graph->num_nodes, file);
    } else {
        fprintf(file, "# Wierzcholki (ID czesc)\n");
        for (int i = 0; i < graph->num_nodes; i++) {
            fprintf(file, "%d %d\n", graph->id_list[i], partition[i]);
        }
        fprintf(file, "\n# Krawedzie (ID1 ID2)\n");
        for (int i = 0; i < graph->num_nodes; i++) {
            for (int j = i + 1; j < graph->num_nodes; j++) {
                if (graph->adjacency_matrix[i][j]) {
                    fprintf(file, "%d %d\n", graph->id_list[i], graph->id_list[j]);
                }
            }
        }
    }
    fclose(file);
}