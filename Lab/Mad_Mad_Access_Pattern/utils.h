#ifndef LOOKUP_UTILS_H
#define LOOKUP_UTILS_H

#include <stdio.h>
#include <stdint.h>

void printTime(const double time);
void openFail(const char *file_name);
void mmapFail(const char *file_name);
void formatFail(const char *file_name);

void printFound(const char *word, const uint32_t count, const float price);
void printNotFound(const char *word);

#endif
