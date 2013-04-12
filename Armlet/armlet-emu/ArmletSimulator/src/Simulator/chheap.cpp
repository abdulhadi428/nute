#include "chheap.h"
#include <stdlib.h>


void *chHeapAlloc(MemoryHeap *heapp, size_t size)
{
    return malloc(size);
}

void chHeapFree(void *p)
{
    free(p);
}
