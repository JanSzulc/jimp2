#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "partition.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_error("Błąd: Nie podano pliku wejściowego", 1);
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];
    int num_parts = 2;
    int margin = 10;
    int binary_output = 0;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            num_parts = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            margin = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            binary_output = 1;
        }
    }

    Graph *graph = load_graph(input_file);
    if (!graph) {
        print_error("Błąd: Nie można wczytać grafu", 2);
    }
    save_graph_as_txt("graf.txt", graph);


    int *partition = partition_graph(graph, num_parts, margin);
    zapisz_wyniki(output_file, partition, graph, binary_output);

    free(partition);
    free_graph(graph);
    return 0;
}