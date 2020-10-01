CC=gcc
CFLAGS=-std=gnu11 -Wall -Werror -Wextra -g -O2
TH=-lpthread
MATH=-lm

all: mmu_test tlb_test simulator

simulator: simulator.o mmu.o page_table.o tlb.o kernel.o
	$(CC) $(CFLAGS) simulator.o mmu.o page_table.o tlb.o kernel.o -o simulator $(TH) $(MATH)

mmu_test : mmu_test.o mmu.o page_table.o tlb.o kernel.o
	$(CC) $(CFLAGS) mmu_test.o mmu.o page_table.o tlb.o kernel.o -o mmu_test $(TH) $(MATH)

tlb_test : tlb_test.o tlb.o
	$(CC) $(CFLAGS) tlb_test.o tlb.o -o tlb_test $(TH) $(MATH)	

simulator.o: simulator.c
	$(CC) -c simulator.c

mmu.o: mmu.c
	$(CC) -c mmu.c

page_table.o: page_table.c
	$(CC) -c page_table.c

tlb.o: tlb.c
	$(CC) -c tlb.c

kernel.o: kernel.c
	$(CC) -c kernel.c

mmu_test.o: mmu_test.c
	$(CC) -c mmu_test.c

tlb_test.o: tlb_test.c
	$(CC) -c tlb_test.c

.PHONY: clean

clean:
	rm -rf mmu_test tlb_test simulator *.o



