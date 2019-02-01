#Jonathan Chang
#CS 3011 C02

CFLAGS = -Wall -g

all: mc1

mc1: mc1.o
	gcc $(CFLAGS) mc1.o -o mc1

mc1.o: mc1.c mc1.h
	gcc $(CFLAGS) -c mc1.c

clean:
	rm -f *.o mc1