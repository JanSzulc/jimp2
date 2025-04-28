#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "utils.h"
#include "partition.h"

// Funkcja wykonujaca odwrotna iteracje do wyznaczania wektorow wlasnych
void inverse_iteration(int** A, int n, double* wektor, int iteracje) {
    double* nowe = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) wektor[i] = 1.0;
    for (int k = 0; k < iteracje; k++) {
        for (int i = 0; i < n; i++) {
            double suma = 0.0;
            for (int j = 0; j < n; j++) {
                if (i != j) suma += A[i][j] * wektor[j];
            }
            nowe[i] = (A[i][i] != 0) ? (wektor[i] - suma) / A[i][i] : 0.0;
        }
        for (int i = 0; i < n; i++) wektor[i] = nowe[i];
    }
    free(nowe);
}

// Funkcja k-means do grupowania wektorow
void kmeans(double* dane, int n, int k, int* przypisania, int wymiar) {
    double** centroidy = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        centroidy[i] = (double*)malloc(wymiar * sizeof(double));
        for (int d = 0; d < wymiar; d++) {
            centroidy[i][d] = dane[(rand() % n) * wymiar + d];
        }
    }

    int zmiana;
    do {
        zmiana = 0;
        for (int i = 0; i < n; i++) {
            double min_odleglosc = 1e9;
            int najlepszy = 0;
            for (int j = 0; j < k; j++) {
                double odleglosc = 0.0;
                for (int d = 0; d < wymiar; d++) {
                    double diff = dane[i * wymiar + d] - centroidy[j][d];
                    odleglosc += diff * diff;
                }
                odleglosc = sqrt(odleglosc);
                if (odleglosc < min_odleglosc) {
                    min_odleglosc = odleglosc;
                    najlepszy = j;
                }
            }
            if (przypisania[i] != najlepszy) {
                przypisania[i] = najlepszy;
                zmiana = 1;
            }
        }

        double** suma = (double**)calloc(k, sizeof(double*));
        int* licznik = (int*)calloc(k, sizeof(int));
        for (int j = 0; j < k; j++) {
            suma[j] = (double*)calloc(wymiar, sizeof(double));
        }

        for (int i = 0; i < n; i++) {
            for (int d = 0; d < wymiar; d++) {
                suma[przypisania[i]][d] += dane[i * wymiar + d];
            }
            licznik[przypisania[i]]++;
        }

        for (int j = 0; j < k; j++) {
            if (licznik[j] > 0) {
                for (int d = 0; d < wymiar; d++) {
                    centroidy[j][d] = suma[j][d] / licznik[j];
                }
            }
        }

        for (int j = 0; j < k; j++) free(suma[j]);
        free(suma);
        free(licznik);
    } while (zmiana);

    for (int i = 0; i < k; i++) free(centroidy[i]);
    free(centroidy);
}

// Funkcja balansujaca rozmiary klastrow
void balansuj_klastry(double* dane, int* przypisania, int liczba_wezlow, int liczba_klastrow, double margines) {
    int* licznik = (int*)calloc(liczba_klastrow, sizeof(int));
    for (int i = 0; i < liczba_wezlow; i++) {
        licznik[przypisania[i]]++;
    }

    double srednia = (double)liczba_wezlow / liczba_klastrow;
    double tolerancja = srednia * (margines / 100.0);
    double min_wielkosc = srednia - tolerancja;
    double max_wielkosc = srednia + tolerancja;

    int poprawione = 1;
    while (poprawione) {
        poprawione = 0;
        for (int i = 0; i < liczba_wezlow; i++) {
            int klaster = przypisania[i];
            if (licznik[klaster] > max_wielkosc) {
                int najlepszy = klaster;
                double min_odleglosc = 1e9;
                for (int k = 0; k < liczba_klastrow; k++) {
                    if (k != klaster && licznik[k] < min_wielkosc) {
                        double odleglosc = fabs(dane[i] - dane[k]);
                        if (odleglosc < min_odleglosc) {
                            min_odleglosc = odleglosc;
                            najlepszy = k;
                        }
                    }
                }
                if (najlepszy != klaster) {
                    licznik[klaster]--;
                    licznik[najlepszy]++;
                    przypisania[i] = najlepszy;
                    poprawione = 1;
                }
            }
        }
    }

    free(licznik);
}

// Funkcja odpowiedzialna za przeprowadzenie spectral clustering
void spectral_clustering(Wezel* lista, int liczba_wezlow, int liczba_klastrow, double margines, Wezel* wezly, const char* wynik, int zapis_bin) {
    int** laplasjan = buduj_laplasjan(lista, liczba_wezlow);

    double* wektory = (double*)malloc(liczba_wezlow * liczba_klastrow * sizeof(double));
    for (int i = 0; i < liczba_klastrow; i++) {
        inverse_iteration(laplasjan, liczba_wezlow, wektory + i * liczba_wezlow, 1000);
    }

    int* przypisania = (int*)calloc(liczba_wezlow, sizeof(int));
    kmeans(wektory, liczba_wezlow, liczba_klastrow, przypisania, liczba_klastrow);

    balansuj_klastry(wektory, przypisania, liczba_wezlow, liczba_klastrow, margines);

    if (zapis_bin)
        zapisz_wynik_bin(wynik, wezly, lista, przypisania, liczba_wezlow);
    else
        zapisz_wynik_txt(wynik, wezly, lista, przypisania, liczba_wezlow);

    free(przypisania);
    free(wektory);
    for (int i = 0; i < liczba_wezlow; i++) free(laplasjan[i]);
    free(laplasjan);
}