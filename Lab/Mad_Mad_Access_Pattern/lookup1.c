#include "tree.h"
#include "utils.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
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

	FILE *fp = fopen(fileName, "rb");
	if (!fp) {
		openFail(fileName);
		return 2;
	} 

	char beginWord[5];
	fread(beginWord, 4, 1, fp);
	if (strncmp("BTRE", beginWord, 4)) {
		formatFail(fileName);
		return 3;
	}

	double startTime = getTime();
	size_t paraLen = sizeof(uint32_t) * 3 + sizeof(float);

	for (int i = 0; i < wordNum; ++i) {
		int index = i + 2;
		fseek(fp, 4, SEEK_SET);
		// search process
		while (true) {
			// root start position		
			int beginPos = ftell(fp);

			fseek(fp, paraLen, SEEK_CUR);
			size_t wordLen = 0;
			while (fgetc(fp) != '\0') {
				++wordLen;
			}

			//get the root node of the tree
			void *root = malloc(paraLen + wordLen + 1);
			fseek(fp, beginPos, SEEK_SET);
			fread(root, paraLen + wordLen + 1, 1, fp);
			char *word = (char *)root + 16;

			int compareRes = strcmp(argv[index], word);
			BinaryTreeNode *node = (BinaryTreeNode *)root;
			if (compareRes == 0) {
				printFound(argv[index], node -> count, node -> price);
				free(root);
				break;
			}
			else if (compareRes < 0) {
				if (node -> left_child) {
					fseek(fp, node -> left_child, SEEK_SET);
					free(root);
				}
				else {
					printNotFound(argv[index]);
					free(root);
					break;
				}
			}
			else {
				if (node -> right_child) {
					fseek(fp, node -> right_child, SEEK_SET);
					free(root);
				}
				else {
					printNotFound(argv[index]);
					free(root);
					break;
				}
			}
		}
	}	

	printTime(getTime() - startTime);

	return 0;
}