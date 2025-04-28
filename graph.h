#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int num_nodes;
    int *id_list;
    int **adjacency_matrix;
} Graph;

Graph* load_graph(const char *filename);
void save_graph_as_txt(const char *filename, Graph *graph);
void free_graph(Graph *graph);

#endif