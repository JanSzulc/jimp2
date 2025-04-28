#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

// Funkcja wczytujaca macierz wezlow z pliku .csrrg
int** wczytaj_macierz_wezlow(FILE* plik, int* rozmiar_macierzy, Wezel** wezly) {
    char linia[1024];

    fgets(linia, sizeof(linia), plik);
    *rozmiar_macierzy = atoi(linia) - 1;

    int** macierz = (int**)malloc(*rozmiar_macierzy * sizeof(int*));
    for (int i = 0; i < *rozmiar_macierzy; i++) {
        macierz[i] = (int*)calloc(*rozmiar_macierzy, sizeof(int));
    }

    *wezly = (Wezel*)calloc((*rozmiar_macierzy) * (*rozmiar_macierzy), sizeof(Wezel));

    // Wczytywanie pozycji wezlow
    fgets(linia, sizeof(linia), plik);
    char* token = strtok(linia, "; \n");
    int* pozycje_wezlow = NULL;
    int liczba_pozycji = 0;

    while (token != NULL) {
        pozycje_wezlow = (int*)realloc(pozycje_wezlow, (liczba_pozycji + 1) * sizeof(int));
        pozycje_wezlow[liczba_pozycji++] = atoi(token);
        token = strtok(NULL, "; \n");
    }

    // Wczytywanie liczby wezlow w wierszach
    fgets(linia, sizeof(linia), plik);
    token = strtok(linia, "; \n");
    int* ilosc_wezlow = NULL;
    int liczba_wierszy = 0;

    while (token != NULL) {
        ilosc_wezlow = (int*)realloc(ilosc_wezlow, (liczba_wierszy + 1) * sizeof(int));
        ilosc_wezlow[liczba_wierszy++] = atoi(token);
        token = strtok(NULL, "; \n");
    }

    // Odtwarzanie macierzy
    int index = 0, id = 0;
    for (int wiersz = 0; wiersz < liczba_wierszy; wiersz++) {
        int liczba_wezlow_w_wierszu = (wiersz == 0) ? ilosc_wezlow[wiersz] : (ilosc_wezlow[wiersz] - ilosc_wezlow[wiersz - 1]);
        for (int i = 0; i < liczba_wezlow_w_wierszu; i++) {
            if (index >= liczba_pozycji) break;
            int kolumna = pozycje_wezlow[index++];
            if (wiersz < *rozmiar_macierzy && kolumna < *rozmiar_macierzy) {
                macierz[wiersz][kolumna] = 1;
                (*wezly)[id].x = kolumna;
                (*wezly)[id].y = wiersz;
                id++;
            }
        }
    }

    free(pozycje_wezlow);
    free(ilosc_wezlow);

    return macierz;
}

// Funkcja wczytujaca liste sasiedztwa z pliku .csrrg
Wezel* wczytaj_liste_sasiedztwa(FILE* plik, int* liczba_wezlow) {
    char* linia1 = wczytaj_cala_linie(plik);
    char* linia2 = wczytaj_cala_linie(plik);

    if (!linia1 || !linia2) {
        printf("Blad wczytywania linii.\n");
        exit(EXIT_FAILURE);
    }

    int* polaczenia = NULL;
    int liczba_polaczen = 0;
    char* token = strtok(linia1, "; \n");
    while (token != NULL) {
        polaczenia = (int*)realloc(polaczenia, (liczba_polaczen + 1) * sizeof(int));
        polaczenia[liczba_polaczen++] = atoi(token);
        token = strtok(NULL, "; \n");
    }

    int* wskazniki = NULL;
    int liczba_wskaznikow = 0;
    token = strtok(linia2, "; \n");
    while (token != NULL) {
        wskazniki = (int*)realloc(wskazniki, (liczba_wskaznikow + 1) * sizeof(int));
        wskazniki[liczba_wskaznikow++] = atoi(token);
        token = strtok(NULL, "; \n");
    }

    int max_wezel = znajdz_maksimum(polaczenia, liczba_polaczen);
    *liczba_wezlow = max_wezel + 1;
    Wezel* lista = (Wezel*)calloc(*liczba_wezlow, sizeof(Wezel));

    int start = 0;
    for (int i = 0; i < liczba_wskaznikow; i++) {
        int koniec = wskazniki[i];
        if (start >= liczba_polaczen) break;
        int wezel = polaczenia[start];
        for (int j = start + 1; j < koniec; j++) {
            if (j >= liczba_polaczen) break;
            lista[wezel].sasiedzi[lista[wezel].liczba_sasiadow++] = polaczenia[j];
        }
        start = koniec;
    }

    if (start < liczba_polaczen) {
        int wezel = polaczenia[start];
        for (int j = start + 1; j < liczba_polaczen; j++) {
            lista[wezel].sasiedzi[lista[wezel].liczba_sasiadow++] = polaczenia[j];
        }
    }

    free(linia1);
    free(linia2);
    free(polaczenia);
    free(wskazniki);

    return lista;
}

// Funkcja zwalniajaca pamiec macierzy
void zwolnij_macierz(int** macierz, int rozmiar) {
    for (int i = 0; i < rozmiar; i++) {
        free(macierz[i]);
    }
    free(macierz);
}

// Funkcja zwalniajaca pamiec listy sasiedztwa
void zwolnij_liste(Wezel* lista) {
    free(lista);
}

// Funkcja budujaca macierz Laplasjana
int** buduj_laplasjan(Wezel* lista, int liczba_wezlow) {
    int** laplasjan = (int**)malloc(liczba_wezlow * sizeof(int*));
    for (int i = 0; i < liczba_wezlow; i++) {
        laplasjan[i] = (int*)calloc(liczba_wezlow, sizeof(int));
    }

    for (int i = 0; i < liczba_wezlow; i++) {
        laplasjan[i][i] = lista[i].liczba_sasiadow;
        for (int j = 0; j < lista[i].liczba_sasiadow; j++) {
            int sasiedzi_idx = lista[i].sasiedzi[j];
            laplasjan[i][sasiedzi_idx] = -1;
        }
    }

    return laplasjan;
}