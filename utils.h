#ifndef UTILS_H
#define UTILS_H

#include "graph.h"

void print_error(const char *message, int exit_code);
void zapisz_wyniki(const char *filename, int *partition, Graph *graph, int binary);

#endif