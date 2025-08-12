#include "kmalloc.h"

typedef struct block_header {
    size_t size;
    struct block_header* next;
    int free;
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)

static uintptr_t heap_start = KERNEL_HEAP_START;
static uintptr_t heap_size = KERNEL_HEAP_SIZE;
static uintptr_t heap_end = KERNEL_HEAP_END;
static block_header_t* free_list = NULL;

void* kmalloc(size_t size) {
    block_header_t* curr = free_list;

    // Find a free block on the list
    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return (void*)((uintptr_t)curr + HEADER_SIZE);
        }
        curr = curr->next;
    }

    // No free block found, allocate new from heap
    if (heap_start + size + HEADER_SIZE > heap_end) {
        return NULL; // Out of memory
    }

        block_header_t* block = (block_header_t*)heap_start;
        block->size = size;
        block->free = 0;
        block->next = free_list;
        free_list = block;

        heap_start += size + HEADER_SIZE;
        return(void*)((uintptr_t)block + HEADER_SIZE);
    
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_header_t* block = (block_header_t*)((uintptr_t)ptr - HEADER_SIZE);
    block->free = 1;
}