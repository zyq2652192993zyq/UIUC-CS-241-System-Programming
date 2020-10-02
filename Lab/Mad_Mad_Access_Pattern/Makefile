CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

all: lookup1 lookup2

lookup1 : lookup1.o utils.o
	$(CC) $(CFLAGS) lookup1.o utils.o -o lookup1

lookup2 : lookup2.o utils.o
	$(CC) $(CFLAGS) lookup2.o utils.o -o lookup2

lookup1.o : lookup1.c
	$(CC) -c lookup1.c

lookup2.o : lookup2.c
	$(CC) -c lookup2.c

utils.o : utils.c
	$(CC) -c utils.c

.PHONY: clean

clean:
	rm -rf lookup1 lookup2 *.o