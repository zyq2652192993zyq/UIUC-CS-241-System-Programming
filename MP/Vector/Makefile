CC=gcc
CFLAGS=-Werror -Wextra -Wall -g -std=gnu11 -lm

ALL: vector_test document_test

document_test: document_test.o document.o vector.o
	$(CC) $(CFLAGS) document_test.o document.o vector.o -o document_test

document_test.o: document_test.c
	$(CC) -c document_test.c

document.o: document.c
	$(CC) -c document.c

vector_test: vector_test.o vector.o
	$(CC) $(CFLAGS) vector_test.o vector.o -o vector_test

vector_test.o: vector_test.c
	$(CC) -c vector_test.c

vector.o: vector.c
	$(CC) -c vector.c
	
clean:
	rm *.o vector_test document_test