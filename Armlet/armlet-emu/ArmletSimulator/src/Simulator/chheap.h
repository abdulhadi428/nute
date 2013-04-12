#ifndef SIM_CHHEAP__H
#define SIM_CHHEAP__H

/**
 * @brief   Allocates a block of memory from the heap by using the first-fit
 *          algorithm.
 * @details The allocated block is guaranteed to be properly aligned for a
 *          pointer data type (@p stkalign_t).
 *
 * @param[in] heapp     pointer to a heap descriptor or @p NULL in order to
 *                      access the default heap.
 * @param[in] size      the size of the block to be allocated. Note that the
 *                      allocated block may be a bit bigger than the requested
 *                      size for alignment and fragmentation reasons.
 * @return              A pointer to the allocated block.
 * @retval NULL         if the block cannot be allocated.
 *
 * @api
 */

typedef void MemoryHeap;

void *chHeapAlloc(MemoryHeap *heapp, size_t size);
void chHeapFree(void *p);

#endif