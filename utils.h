#ifndef UTILS_H
#define UTILS_H

void print_error(const char *message, int exit_code);
void save_partition(const char *filename, int *partition, int num_nodes, int binary);

#endif
