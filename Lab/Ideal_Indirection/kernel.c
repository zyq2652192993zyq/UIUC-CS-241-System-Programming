#include "kernel.h"
#include "mmu.h"

void *ask_kernel_for_frame() { return malloc(sizeof(PageTable)); }
