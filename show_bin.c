#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Użycie: %s plik.bin\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Błąd otwarcia pliku");
        return 2;
    }

    int value;
    int index = 0;
    while (fread(&value, sizeof(int), 1, file)) {
        printf("Wierzchołek %d: część %d\n", index++, value);
    }

    fclose(file);
    return 0;
}
