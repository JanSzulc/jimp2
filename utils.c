#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void print_error(const char *message, int exit_code) {
    fprintf(stderr, "%s\n", message);
    exit(exit_code);
}

void save_partition(const char *filename, int *partition, int num_nodes, int binary) {
    FILE *file;
    if (binary) {
        file = fopen(filename, "wb");
        if (!file) print_error("Błąd: Nie można otworzyć pliku do zapisu (binarnie)", 3);
        fwrite(partition, sizeof(int), num_nodes, file);
    } else {
        file = fopen(filename, "w");
        if (!file) print_error("Błąd: Nie można otworzyć pliku do zapisu", 3);
        for (int i = 0; i < num_nodes; i++) {
            fprintf(file, "%d\n", partition[i]);
        }
    }
    fclose(file);
}
