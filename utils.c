#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Funkcja wczytujaca cala linie tekstu z pliku
char* wczytaj_cala_linie(FILE* plik) {
    size_t size = 1024;
    char* linia = (char*)malloc(size);
    if (!linia) return NULL;

    size_t len = 0;
    int c;
    while ((c = fgetc(plik)) != EOF && c != '\n') {
        linia[len++] = (char)c;
        if (len == size) {
            size *= 2;
            linia = (char*)realloc(linia, size);
            if (!linia) return NULL;
        }
    }
    linia[len] = '\0';
    return linia;
}

// Funkcja zwracajaca maksimum z tablicy
int znajdz_maksimum(int* tablica, int n) {
    int max = tablica[0];
    for (int i = 1; i < n; i++) {
        if (tablica[i] > max) max = tablica[i];
    }
    return max;
}

// Funkcja zapisujaca wyniki do pliku tekstowego
void zapisz_wynik_txt(const char* filename, Wezel* wezly, Wezel* lista, int* przypisania, int liczba_wezlow) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Nie mozna otworzyc pliku wynikowego TXT");
        return;
    }

    for (int id = 0; id < liczba_wezlow; id++) {
        fprintf(f, "%d;%d;%d;%d;", id, wezly[id].x, wezly[id].y, przypisania[id]);
        for (int i = 0; i < lista[id].liczba_sasiadow; i++) {
            fprintf(f, "%d", lista[id].sasiedzi[i]);
            if (i < lista[id].liczba_sasiadow - 1)
                fprintf(f, ",");
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

// Funkcja zapisujaca wyniki do pliku binarnego
void zapisz_wynik_bin(const char* filename, Wezel* wezly, Wezel* lista, int* przypisania, int liczba_wezlow) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("Nie mozna otworzyc pliku wynikowego BIN");
        return;
    }

    fwrite(&liczba_wezlow, sizeof(int), 1, f);

    for (int id = 0; id < liczba_wezlow; id++) {
        fwrite(&id, sizeof(int), 1, f);
        fwrite(&wezly[id].x, sizeof(int), 1, f);
        fwrite(&wezly[id].y, sizeof(int), 1, f);
        fwrite(&przypisania[id], sizeof(int), 1, f);
        fwrite(&lista[id].liczba_sasiadow, sizeof(int), 1, f);
        fwrite(lista[id].sasiedzi, sizeof(int), lista[id].liczba_sasiadow, f);
    }

    fclose(f);
}