CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

all: semamoreTest rendezvous

semamoretest: semamore.o next_permutation.o semamoreTest.o 
	$(CC) $(CFLAGS) semamore.o next_permutation.o semamoreTest.o -o semamoreTest $(th) $(math)

semamore.o: semamore.c
	$(CC) semamore.c

next_permutation.o: next_permutation.c
	$(CC) next_permutation.c

semamoretest.o: semamoreTest.c
	$(CC) semamoretest.c

rendezvous: rendezvous.o
	$(CC) $(CFLAGS) rendezvous.o -o rendezvous $(th) $(math)

rendezvous.o: rendezvous.c
	$(CC) -c rendezvous.c

.PHONY: clean

clean:
	rm -rf rendezvous semamoreTest *.o
