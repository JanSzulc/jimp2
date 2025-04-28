#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int num_nodes;
    int **adjacency_matrix;
} Graph;

Graph* load_graph(const char *filename);
void free_graph(Graph *graph);

#endif
