CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

count_primes: count_primes.o
	$(CC) $(CFLAGS) count_primes.o -o count_primes $(TH) $(MATH)

count_primes.o: count_primes.c
	$(CC) -c count_primes.c

.PHONY: clean

clean:
	rm -rf count_primes count_primes.o
