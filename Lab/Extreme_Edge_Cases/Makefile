CC=gcc
CFLAGS=-std=gnu11 -lm -Wall -Werror -Wextra -g -O2

camelCaser_main: camelCaser_main.o camelCaser_tests.o camelCaser.o
	$(CC) $(CFLAGS) camelCaser_main.o camelCaser_tests.o camelCaser.o -o camelCaser_main

camelCaser_main.o: camelCaser_main.c
	$(CC) -c camelCaser_main.c

camelCaser_tests.o: camelCaser_tests.c
	$(CC) -c camelCaser_tests.c

camelCaser.o: camelCaser.c
	$(CC) -c camelCaser.c

clean:
	rm *.o camelCaser_main
