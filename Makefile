# Jonathan Chang
# CS 3011 C02
# Project 1 

CFLAGS = -Wall -g

all: mc0 mc1 mc2

mc0: v0/mc0.o
	gcc $(CFLAGS) v0/mc0.o -o mc0
	gcc $(CFLAGS) v0/mc0.o -o v0/mc0

mc0.o: v0/mc0.c v0/mc0.h
	gcc $(CFLAGS) -c v0/mc0.c

mc1: v1/mc1.o
	gcc $(CFLAGS) v1/mc1.o -o mc1
	gcc $(CFLAGS) v1/mc1.o -o v1/mc1

mc1.o: v1/mc1.c v1/mc1.h
	gcc $(CFLAGS) -c v1/mc1.c

mc2: v2/mc2.o
	gcc $(CFLAGS) v2/mc2.o -o mc2
	gcc $(CFLAGS) v2/mc2.o -o v2/mc2

mc2.o: v2/mc2.c v2/mc2.h
	gcc $(CFLAGS) -c v2/mc2.c

clean:
	rm -f v0/*.o mc0 v0/mc0
	rm -f v1/*.o mc1 v1/mc1
	rm -f v2/*.o mc2 v2/mc2

