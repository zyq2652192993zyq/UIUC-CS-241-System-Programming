CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

all: barrierTest

barrierTest: barrier.o algorithm.o barrierTest.o
	$(CC) $(CFLAGS) barrier.o algorithm.o barrierTest.o -o barrierTest $(TH) $(MATH)

barrier.o: barrier.c
	$(CC) -c barrier.c

algorithm.o: algorithm.c
	$(CC) -c algorithm.c

.PHONY: clean

clean:
	rm -rf barrierTest *.o
