CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

all: barrier_test poisson_test

barrier_test: barrier.o algorithm.o barrier_test.o
	$(CC) $(CFLAGS) barrier.o algorithm.o barrier_test.o -o barrier_test $(TH) $(MATH)

barrier.o: barrier.c
	$(CC) -c barrier.c

algorithm.o: algorithm.c
	$(CC) -c algorithm.c

poisson_test: barrier.o poisson.o bmp.o poisson_test.o
	$(CC) $(CFLAGS) barrier.o poisson.o bmp.o poisson_test.o -o poisson_test $(TH) $(MATH)

poisson.o: poisson.c
	$(CC) -c poisson.c

bmp.o: bmp.c
	$(CC) -c bmp.c

poisson_test.o: poisson_test.c
	$(CC) -c poisson_test.c


.PHONY: clean

clean:
	rm -rf barrier_test poisson_test *.o
