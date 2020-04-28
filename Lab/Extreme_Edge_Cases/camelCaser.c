#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include "camelCaser.h"

bool print_output(char **output, char **input)
{
    
}

char ** camel_caser(const char* input)
{
    int cnt = 0;
	int len = strlen(input);
	//统计标点符号个数，也是子串的个数
	for (int i = 0; i < len; ++i) {
		if (ispunct(input[i])) ++(cnt);
	}
	//切割出每个子串
	int num = cnt;
	char **output = malloc(num * sizeof(char *));
	int start = 0, end = 0;
	int pos = 0;
	while (end < len) {
		while (!ispunct(input[end])) { ++end; }
		int n = end - start;
		output[pos] = malloc(n * sizeof(char));
		for (int i = 0; i < n; ++i) {
			output[pos][i] = input[i + start];
		}
		end += 2; start = end; ++pos;
	}

	char **res = malloc(num * sizeof(char *)); //最终的结果
	for (int i = 0; i < num; ++i) {
		char *tmp = output[i];
		int size = strlen(tmp);
		int k = 0;
		for (int j = 0; j < size; ++j) {
			if (isspace(tmp[j])) ++k;
		}
		res[i] = malloc((size - k) * sizeof(char));
		res[i][0] = tolower(tmp[0]);
		int pos = 1;
		for (int j = 1; j < size; ++j) {
			if (isspace(tmp[j])) {
				tmp[j + 1] = toupper(tmp[j + 1]);
			}
			else {
				res[i][pos++] = tmp[j];
			}
		}
	}

	//for (int i = 0; i < num; ++i) {
	//	free(output[i]); output[i] = NULL;
	//}
	free(output); output = NULL;

	return res;
}

void makeEmpty(char **output)
{
	while (*output) {
		char *tmp = *output;
		++output;
		free(tmp); tmp = NULL;
	}
}
