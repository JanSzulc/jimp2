CC=gcc
CFLAGS=-Wall -Wextra -std=c11
OBJ=main.o graph.o partition.o utils.o

graph_partition: $(OBJ)
	$(CC) $(CFLAGS) -o graph_partition $(OBJ)

main.o: main.c graph.h partition.h utils.h
graph.o: graph.c graph.h
partition.o: partition.c partition.h graph.h
utils.o: utils.c utils.h

clean:
	rm -f *.o graph_partition
