CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

all: semamoreTest rendezvous queueTest

queueTest: queue.o queueTest.o
	$(CC) $(CFLAGS) queue.o queueTest.o -o queueTest $(TH) $(MATH)

queueTest.o: queueTest.c
	$(CC) -c queueTest.c

queue.o: queue.c
	$(CC) -c queue.c

semamoreTest: semamore.o algorithm.o semamoreTest.o 
	$(CC) $(CFLAGS) semamore.o algorithm.o semamoreTest.o -o semamoreTest $(TH) $(MATH)

semamore.o: semamore.c
	$(CC) -c semamore.c

algorithm.o: algorithm.c
	$(CC) -c algorithm.c

semamoretest.o: semamoreTest.c
	$(CC) -c semamoretest.c

rendezvous: rendezvous.o
	$(CC) $(CFLAGS) rendezvous.o -o rendezvous $(TH) $(MATH)

rendezvous.o: rendezvous.c
	$(CC) -c rendezvous.c

.PHONY: clean

clean:
	rm -rf rendezvous semamoreTest queueTest *.o
