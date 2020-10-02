#include "utils.h"

void printTime(const double time) {
	printf("Execution time:%f seconds\n", time);
}

void openFail(const char *file_name) {
	fprintf(stderr, "Cannot open %s\n", file_name);
}

void mmapFail(const char *file_name) {
	fprintf(stderr, "Failed to map %s\n", file_name);
}

void formatFail(const char *file_name) {
	fprintf(stderr, "%s: Incorrect format\n", file_name);
}

void printFound(const char *word, const uint32_t count, const float price) {
	printf("%s: %u at $%.2f\n", word, count, price);
}

void printNotFound(const char *word) { printf("%s not found\n", word); }
