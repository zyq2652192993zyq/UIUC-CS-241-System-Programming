#ifndef _PRINT_H
#define _PRINT_H
#include "mini_valgrind.h"

extern void print_leak_info(meta_data *head, size_t total_usage, size_t total_free, size_t bad_frees);

#endif
