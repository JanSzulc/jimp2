// File: src/main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 1024

typedef struct Wezel {
    int sasiedzi[100];
    int liczba_sasiadow;
    int x;
    int y;
} Wezel;

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

int znajdz_maksimum(int* tablica, int n) {
    int max = tablica[0];
    for (int i = 1; i < n; i++) {
        if (tablica[i] > max) max = tablica[i];
    }
    return max;
}

int** wczytaj_macierz_wezlow(FILE* plik, int* rozmiar_macierzy, Wezel** wezly) {
    char linia[MAX_LINE_LENGTH];

    fgets(linia, sizeof(linia), plik);
    *rozmiar_macierzy = atoi(linia) - 1;

    int** macierz = (int**)malloc(*rozmiar_macierzy * sizeof(int*));
    for (int i = 0; i < *rozmiar_macierzy; i++) {
        macierz[i] = (int*)calloc(*rozmiar_macierzy, sizeof(int));
    }

    *wezly = (Wezel*)calloc((*rozmiar_macierzy) * (*rozmiar_macierzy), sizeof(Wezel));

    fgets(linia, sizeof(linia), plik);
    char* token = strtok(linia, "; \n");
    int* pozycje_wezlow = NULL;
    int liczba_pozycji = 0;

    while (token != NULL) {
        pozycje_wezlow = (int*)realloc(pozycje_wezlow, (liczba_pozycji + 1) * sizeof(int));
        pozycje_wezlow[liczba_pozycji++] = atoi(token);
        token = strtok(NULL, "; \n");
    }

    fgets(linia, sizeof(linia), plik);
    token = strtok(linia, "; \n");
    int* ilosc_wezlow = NULL;
    int liczba_wierszy = 0;

    while (token != NULL) {
        ilosc_wezlow = (int*)realloc(ilosc_wezlow, (liczba_wierszy + 1) * sizeof(int));
        ilosc_wezlow[liczba_wierszy++] = atoi(token);
        token = strtok(NULL, "; \n");
    }

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

    // Automatyczne domykanie brakujacych polaczen
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

void wypisz_macierz(int** macierz, int rozmiar) {
    for (int i = 0; i < rozmiar; i++) {
        for (int j = 0; j < rozmiar; j++) {
            printf("%d ", macierz[i][j]);
        }
        printf("\n");
    }
}

void wypisz_liste_sasiedztwa(Wezel* lista, int liczba_wezlow) {
    for (int i = 0; i < liczba_wezlow; i++) {
        if (lista[i].liczba_sasiadow > 0) {
            printf("Wezel %d: ", i);
            for (int j = 0; j < lista[i].liczba_sasiadow; j++) {
                printf("%d", lista[i].sasiedzi[j]);
                if (j < lista[i].liczba_sasiadow - 1) printf(", ");
            }
            printf("\n");
        }
    }
}

void zwolnij_macierz(int** macierz, int rozmiar) {
    for (int i = 0; i < rozmiar; i++) {
        free(macierz[i]);
    }
    free(macierz);
}

void zwolnij_liste(Wezel* lista) {
    free(lista);
}

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

void inverse_iteration(int** A, int n, double* wektor, int iteracje) {
    double* nowe = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) wektor[i] = 1.0;
    for (int k = 0; k < iteracje; k++) {
        for (int i = 0; i < n; i++) {
            double suma = 0.0;
            for (int j = 0; j < n; j++) {
                if (i != j) suma += A[i][j] * wektor[j];
            }
            if (A[i][i] != 0)
                nowe[i] = (wektor[i] - suma) / A[i][i];
            else
                nowe[i] = 0.0;
        }
        for (int i = 0; i < n; i++) wektor[i] = nowe[i];
    }
    free(nowe);
}

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

void popraw_przypisania_na_sasiedztwo(Wezel* lista, int* przypisania, int liczba_wezlow, int liczba_klastrow) {
    for (int i = 0; i < liczba_wezlow; i++) {
        int klaster = przypisania[i];
        int licznik = 0;
        for (int j = 0; j < lista[i].liczba_sasiadow; j++) {
            if (przypisania[lista[i].sasiedzi[j]] == klaster) {
                licznik++;
            }
        }
        if (licznik == 0) {
            int najlepszy_klaster = klaster;
            int max_sasiadow = -1;
            for (int k = 0; k < liczba_klastrow; k++) {
                if (k == klaster) continue;
                int licznik_sasiadow = 0;
                for (int j = 0; j < lista[i].liczba_sasiadow; j++) {
                    if (przypisania[lista[i].sasiedzi[j]] == k) {
                        licznik_sasiadow++;
                    }
                }
                if (licznik_sasiadow > max_sasiadow) {
                    max_sasiadow = licznik_sasiadow;
                    najlepszy_klaster = k;
                }
            }
            przypisania[i] = najlepszy_klaster;
        }
    }
}

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


int main(int argc, char* argv[]) {
    if (argc < 3 || argv[2][0] == '-') {
        printf("Blad: nie podano pliku wynikowego lub podano nieprawidlowy plik wynikowy.\n");
        printf("Uzycie: %s <plik.csrrg> <plik_wynikowy> [-p liczba_czesci] [-m margines_procentowy] [-b]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int liczba_klastrow = 2;
    double margines = 10.0;
    int zapis_bin = 0;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            liczba_klastrow = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            margines = atof(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            zapis_bin = 1;
        }
    }

    FILE* plik = fopen(argv[1], "r");
    if (!plik) {
        perror("Nie mozna otworzyc pliku");
        return EXIT_FAILURE;
    }

    int rozmiar;
    Wezel* wezly;
    int** macierz = wczytaj_macierz_wezlow(plik, &rozmiar, &wezly);
    printf("Macierz wezlow:\n");
    wypisz_macierz(macierz, rozmiar);

    Wezel* lista = wczytaj_liste_sasiedztwa(plik, &rozmiar);
    printf("\nLista sasiedztwa:\n");
    wypisz_liste_sasiedztwa(lista, rozmiar);

    spectral_clustering(lista, rozmiar, liczba_klastrow, margines, wezly, argv[2], zapis_bin);

    zwolnij_macierz(macierz, rozmiar);
    zwolnij_liste(lista);
    free(wezly);
    fclose(plik);

    return EXIT_SUCCESS;
}