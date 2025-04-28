#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "partition.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    // Sprawdzenie poprawnosci argumentow wywolania
    if (argc < 3 || argv[2][0] == '-') {
        printf("Blad: nie podano pliku wynikowego lub podano nieprawidlowy plik wynikowy.\n");
        printf("Uzycie: %s <plik.csrrg> <plik_wynikowy> [-p liczba_czesci] [-m margines_procentowy] [-b]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Domyslne wartosci parametrow
    int liczba_klastrow = 2;
    double margines = 10.0;
    int zapis_bin = 0;

    // Przetwarzanie opcjonalnych argumentow
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            liczba_klastrow = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            margines = atof(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            zapis_bin = 1;
        }
    }

    // Otwarcie pliku wejściowego
    FILE* plik = fopen(argv[1], "r");
    if (!plik) {
        perror("Nie mozna otworzyc pliku");
        return EXIT_FAILURE;
    }

    int rozmiar;
    Wezel* wezly;

    // Wczytanie macierzy wezlow z pliku
    int** macierz = wczytaj_macierz_wezlow(plik, &rozmiar, &wezly);
    printf("Macierz wezlow:\n");
    wypisz_macierz(macierz, rozmiar);

    // Wczytanie listy sasiedztwa
    Wezel* lista = wczytaj_liste_sasiedztwa(plik, &rozmiar);
    printf("\nLista sasiedztwa:\n");
    wypisz_liste_sasiedztwa(lista, rozmiar);

    // Wlasciwy podzial grafu na klastry
    spectral_clustering(lista, rozmiar, liczba_klastrow, margines, wezly, argv[2], zapis_bin);

    // Zwolnienie zaalokowanej pamieci
    zwolnij_macierz(macierz, rozmiar);
    zwolnij_liste(lista);
    free(wezly);
    fclose(plik);

    return EXIT_SUCCESS;
}