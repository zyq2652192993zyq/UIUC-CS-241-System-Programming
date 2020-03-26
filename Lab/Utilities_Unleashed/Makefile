CC=gcc
CFLAGS=-std=gnu11 -Werror -Wextra -Wall -g

time: time.o format.o
	$(CC) $(CFLAGS) time.o format.o -o time

time.o: time.c
	$(CC) -c time.c

format.o: format.c
	$(CC) -c format.c

clean:
	rm *.o time
