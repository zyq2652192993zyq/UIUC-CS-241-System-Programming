#include "algorithm.h"

void reverse(void *seq, int begin, int end)
{
	int *vec = (int *)seq;
    int tail = end - 1;

	while (begin <= tail) {
		swap((void *)&vec[begin], (void *)&vec[tail]);
        ++begin, --tail;
	}
}

bool next_permutation(void *seq, int begin, int end)
{
	int *vec = (int *)seq;

	if (begin == end) return false; // empty array
	int i = begin;
	++i;
	if (i == end) return false; // only one element
	i = end;
	--i;

	while (true) {
		int ii = i;
		--i;

		if (vec[i] < vec[ii]) {
			int j = end;
			while (!(vec[i] < vec[--j]));
			swap((void *)&vec[i], (void *)&vec[j]);
			reverse(seq, ii, end);

			return true;
		}

		if (i == begin) {
			reverse(seq, begin, end);
			return false;
		}
	}
}

void swap(void *a, void *b)
{
	int *numA = (int *)a;
	int *numB = (int *)b;

	int tmp = *numA;
	*numA = *numB;
	*numB = tmp;
}

