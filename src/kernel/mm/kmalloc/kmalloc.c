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

uint16_t minimum_split_size = HEADER_SIZE + 8;

void heap_init() {
    free_list = (block_header_t*)heap_start;
    free_list->size = heap_size - HEADER_SIZE;
    free_list->free = 1;
    free_list->next = NULL;

    LOG_INFO("Initialized Heap");
    LOG_INFO_NONL("Heap size: "); draw_dec(heap_size);
    newLineFB();
}

void* kmalloc(size_t size) {
    block_header_t* curr = free_list;
    block_header_t* prev = NULL;

    while (curr) {
        if (curr->free && curr->size >= size) {
            // If big enough to split, split
            if (curr->size >= size + HEADER_SIZE + minimum_split_size) {
                block_header_t* new_block = (block_header_t*)((uintptr_t)curr + HEADER_SIZE + size);
                new_block->size = curr->size - size - HEADER_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->free = 0;
                curr->next = new_block;
                return (void*)((uintptr_t)curr + HEADER_SIZE);
            }
            // Otherwise, use entire block
            curr->free = 0;
            return (void*)((uintptr_t)curr + HEADER_SIZE);
        }
        prev = curr;
        curr = curr->next;
    }

    // No suitable free block found, try to allocate from heap bump pointer
    if (heap_start + size + HEADER_SIZE > heap_end) {
        return NULL; // Out of memory
    }

    block_header_t* block = (block_header_t*)heap_start;
    block->size = size;
    block->free = 0;
    block->next = NULL;

    heap_start += size + HEADER_SIZE;

    // Add to free list
    if (prev) {
        prev->next = block;
    } else {
        free_list = block;
    }

    return (void*)((uintptr_t)block + HEADER_SIZE);
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_header_t* block = (block_header_t*)((uintptr_t)ptr - HEADER_SIZE);
    block->free = 1;

    // Coalesce with next block if free
    if (block->next && block->next->free) {
        block->size += HEADER_SIZE + block->next->size;
        block->next = block->next->next;
    }
}

void* kmalloc_a(size_t size, size_t align) {
    uintptr_t raw = (uintptr_t) kmalloc(size + align + sizeof(uintptr_t));
    uintptr_t aligned = (raw + sizeof(uintptr_t) + (align - 1)) & ~(align - 1);

    // Store original pointer just before aligned one
    ((uintptr_t*)aligned)[-1] = raw;

    return (void*)aligned;
}

void kfree_a(void* ptr) {
    uintptr_t raw = ((uintptr_t*)ptr)[-1];
    kfree((void*)raw);
}