CC = gcc
CFLAGS = -Werror -Wextra -Wall -g -std=gnu11 -lm

ALL: part1-main part2-main

part1-main: part1-main.o part1-functions.o
	$(CC) $(CFLAGS) part1-main.o part1-functions.o -o part1-main

part1-main.o: part1-main.c
	$(CC) -c part1-main.c

part1-functions.o: part1-functions.c
	$(CC) -c part1-functions.c

part2-main: part2-main.o part2-functions.o
	$(CC) $(CFLAGS) part2-main.o part2-functions.o -o part2-main

part2-main.o: part2-main.c
	$(CC) -c part2-main.c

part2-functions.o: part2-functions.c
	$(CC) -c part2-functions.c

clean:
	rm *.o part1-main part2-main
