#ifndef PARTITION_H
#define PARTITION_H

#include "graph.h"

// Funkcja wykonujaca spectral clustering i zapisujaca wynik
void spectral_clustering(Wezel* lista, int liczba_wezlow, int liczba_klastrow, double margines, Wezel* wezly, const char* wynik, int zapis_bin);

#endif // PARTITION_H