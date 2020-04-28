CC=gcc
CFLAGS=-Werror -Wall -Wextra -O2 -g -std=c++11

test: test.o mini_valgrind.o print.o
	$(CC) $(CFLAGS) test.o mini_valgrind.o  print.o -o test

test.o: test.c
	$(CC) -c test.c

mini_valgrind.o: mini_valgrind.c
	$(CC) -c mini_valgrind.c

print.o: print.c
	$(CC) -c print.c

clean:
	rm *.o test