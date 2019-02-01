# Jonathan Chang
# CS 3011 C02
# Project 1 

CFLAGS = -Wall -g

all: mc2

mc2: mc2.o
	gcc $(CFLAGS) mc2.o -o mc2

mc2.o: mc2.c mc2.h
	gcc $(CFLAGS) -c mc2.c

clean:
	rm -f *.o mc2