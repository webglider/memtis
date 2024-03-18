CC = gcc
CFLAGS = -O3 -msse4 -g -march=native -funroll-loops

all: gups64-rw

gups64-rw:
	$(CC) $(CFLAGS) gups-opt.c -o gups64-rw -lpthread

clean:
	rm -f gups64-rw *.o
