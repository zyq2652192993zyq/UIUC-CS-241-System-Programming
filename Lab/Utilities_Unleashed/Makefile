CC=gcc
CFLAGS=-std=gnu11 -Werror -Wextra -Wall -g

ALL:time env

time: time.o format.o
	$(CC) $(CFLAGS) time.o format.o -o time

time.o: time.c
	$(CC) -c time.c

env: env.o format.o
	$(CC) $(CFLAGS) env.o format.o -o env

env.o: env.c
	$(CC) -c env.c

format.o: format.c
	$(CC) -c format.c

clean:
	rm *.o time env
