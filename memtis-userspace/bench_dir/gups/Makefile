CC = gcc
CFLAGS = -O3 -msse4 -g -march=native -funroll-loops

all: gups-r

gups-r:
	$(CC) $(CFLAGS) gups.c -o gups-r -lpthread

clean:
	rm -f gups-r *.o
