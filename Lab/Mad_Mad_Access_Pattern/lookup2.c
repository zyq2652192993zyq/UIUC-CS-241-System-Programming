#include "tree.h"
#include "utils.h"
#include <sys/mman.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup2 <data_file> <word> [<word> ...]
*/

double getTime() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);

	return t.tv_sec + t.tv_nsec / 1e9;
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("\n\t./lookup1 <data_file> <word> [<word> ...]\n");
		return 1;
	}

	const char *fileName = argv[1];
	int wordNum = argc - 2;

	int fd = open(fileName, O_RDONLY);
	if (fd < 0) {
		openFail(fileName);
		return 2;
	}

	struct stat st;
	fstat(fd, &st);
	size_t len = st.st_size;
	void *address = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);

	if (strncmp("BTRE", (char *)address, 4)) {
		formatFail(fileName);
		return 3;
	}

	double startTime = getTime();
	size_t paraLen = sizeof(uint32_t) * 3 + sizeof(float);

	for (int i = 0; i < wordNum; ++i) {
		int index = i + 2;
		void *curAddress = address + 4;

		//search process
		while (true) {
			BinaryTreeNode * node = (BinaryTreeNode *) curAddress;
			int compareRes = strcmp(argv[index], curAddress + 16);
			if (compareRes == 0) {
				printFound(argv[index], node -> count, node -> price);
				break;
			}
			else if (compareRes < 0) {
				if (node -> left_child) {
					curAddress = address + node -> left_child;
				}
				else {
					printNotFound(argv[index]);
					break;
				}
			}
			else {
				if (node -> right_child) {
					curAddress = address + node -> right_child;
				}
				else {
					printNotFound(argv[index]);
					break;
				}
			}
		}
	}

	printTime(getTime() - startTime);

	munmap(address, len);

	return 0;
}