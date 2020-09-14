CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -Wno-error=unused-parameter -g -O2
TH=-lpthread
MATH=-lm

all: simulator

simulator: simulator.o good_company.o company.o
	$(CC) $(CFLAGS) company.o good_company.o simulator.o -o simulator $(TH) $(MATH)


company.o: company.c
	$(CC) -c company.c

good_company.o: good_company.c
	$(CC) -c good_company.c


.PHONY: clean

clean:
	rm -rf simulator *.o