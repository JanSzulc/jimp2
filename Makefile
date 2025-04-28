# Kompilator i flagi
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Pliki zrodlowe
SRCS = main.c graph.c partition.c utils.c

# Pliki obiektowe
OBJS = main.o graph.o partition.o utils.o

# Wynikowy plik wykonywalny
TARGET = graph_partition.exe

# Domyslne zadanie
all: $(TARGET)

# Kompilacja programu
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Kompilacja pojedynczych plikow obiektowych
main.o: main.c graph.h partition.h utils.h
	$(CC) $(CFLAGS) -c main.c

graph.o: graph.c graph.h utils.h
	$(CC) $(CFLAGS) -c graph.c

partition.o: partition.c partition.h graph.h utils.h
	$(CC) $(CFLAGS) -c partition.c

utils.o: utils.c utils.h graph.h
	$(CC) $(CFLAGS) -c utils.c

# Czyszczenie
clean:
	del /q *.o *.exe

.PHONY: all clean