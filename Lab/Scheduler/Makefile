CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -O2 -std=gnu11
LFLAGS=-lpthread

simulator : simulator.o libscheduler.o libpriqueue.o
	$(CC) $(CFLAGS) simulator.o libscheduler.o libpriqueue.o -o simulator $(LFLAGS)

simulator.o : simulator.c
	$(CC) -c simulator.c

libpriqueue.o : libpriqueue.c
	$(CC) -c libpriqueue.c

libscheduler.o : libscheduler.c
	$(CC) -c libscheduler.c


.PHONY: clean

clean:
	rm *.o simulator

