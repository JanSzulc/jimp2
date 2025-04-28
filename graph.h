#ifndef GRAPH_H
#define GRAPH_H

// Struktura reprezentujaca wezel grafu
typedef struct Wezel {
    int sasiedzi[100];    // Lista sasiadow
    int liczba_sasiadow;  // Liczba sasiadow
    int x, y;             // Pozycja wezla w macierzy
} Wezel;

// Funkcje operujace na grafie
int** wczytaj_macierz_wezlow(FILE* plik, int* rozmiar_macierzy, Wezel** wezly);
Wezel* wczytaj_liste_sasiedztwa(FILE* plik, int* liczba_wezlow);
void wypisz_macierz(int** macierz, int rozmiar);
void wypisz_liste_sasiedztwa(Wezel* lista, int liczba_wezlow);
void zwolnij_macierz(int** macierz, int rozmiar);
void zwolnij_liste(Wezel* lista);
int** buduj_laplasjan(Wezel* lista, int liczba_wezlow);

#endif // GRAPH_H