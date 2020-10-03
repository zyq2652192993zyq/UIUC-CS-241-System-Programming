CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread

brutus : brutus.o
	$(CC) $(CFLAGS) brutus.o -o brutus $(TH)

brutus.o : brutus.c
	$(CC) -c brutus.c

.PHONY: clean

clean:
	rm -rf brutus *.o