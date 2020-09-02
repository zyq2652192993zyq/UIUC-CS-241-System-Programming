CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

rendezvous: rendezvous.o
	$(CC) $(CFLAGS) rendezvous.o -o rendezvous $(TH) $(MATH)

rendezvous.o: rendezvous.c
	$(CC) -c rendezvous.c

.PHONY: clean

clean:
	rm -rf rendezvous rendezvous.o
