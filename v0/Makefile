#Jonathan Chang
#CS 3011 C02

CFLAGS = -Wall -g

all: mc0

mc0: mc0.o
	gcc $(CFLAGS) mc0.o -o mc0

mc0.o: mc0.c mc0.h
	gcc $(CFLAGS) -c mc0.c

clean:
	rm -f *.o mc0