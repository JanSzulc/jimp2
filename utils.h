#ifndef UTILS_H
#define UTILS_H

#include "graph.h"

// Funkcja wczytujaca cala linie z pliku
char* wczytaj_cala_linie(FILE* plik);

// Funkcja znajdujaca maksimum w tablicy
int znajdz_maksimum(int* tablica, int n);

// Funkcja zapisujaca wynik do pliku tekstowego
void zapisz_wynik_txt(const char* filename, Wezel* wezly, Wezel* lista, int* przypisania, int liczba_wezlow);

// Funkcja zapisujaca wynik do pliku binarnego
void zapisz_wynik_bin(const char* filename, Wezel* wezly, Wezel* lista, int* przypisania, int liczba_wezlow);

#endif // UTILS_H