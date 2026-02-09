#include "kmalloc.h"
#include "logf/logf.h"
#include "util/util.h"
#include "drivers/com_1/com_1.h"
#include "panic/panic_codes.h"

#define ALIGN8(x) (((x) + 7) & ~7)
#define MIN_BLOCK_SIZE 8
#define HEADER_SIZE sizeof(block_header_t)
#define HEAP_MAGIC 0xC0FFEE42

// Kernel heap boundaries
static uintptr_t heap_start = KERNEL_HEAP_START;
static uintptr_t heap_size = KERNEL_HEAP_SIZE;
static uintptr_t heap_end = KERNEL_HEAP_END;

static block_header_t *heap_head = NULL;

#define HEAP_ASSERT(cond, string, code) \
    do { \
        if (!(cond)) { \
            panic(string, (code)); \
        } \
    } while (0)

void heap_validate(void) {
    block_header_t *b = heap_head;

    while (b) {
        // Header integrity
        HEAP_ASSERT(
            b->magic == HEAP_MAGIC,
            "heap: block header magic corrupted",
            PANIC_HEAP_MAGIC_CORRUPTION
        );

        HEAP_ASSERT(
            b->size > 0,
            "heap: block has invalid size",
            PANIC_HEAP_INVALID_SIZE
        );

        // Linked list integrity
        HEAP_ASSERT(
            !b->next || b->next->prev == b,
            "heap: next->prev link corrupted",
            PANIC_HEAP_BAD_NEXT_LINK
        );

        HEAP_ASSERT(
            !b->prev || b->prev->next == b,
            "heap: prev->next link corrupted",
            PANIC_HEAP_BAD_PREV_LINK
        );

        // Heap bounds
        HEAP_ASSERT(
            (uintptr_t)b >= heap_start,
            "heap: block header below heap start",
            PANIC_HEAP_OUT_OF_BOUNDS
        );

        HEAP_ASSERT(
            (uintptr_t)b + HEADER_SIZE + b->size <= heap_end,
            "heap: block exceeds heap end",
            PANIC_HEAP_OUT_OF_BOUNDS
        );

        b = b->next;
    }
}

void heap_init(void) {
    heap_head = (block_header_t *)heap_start;
    heap_head->magic = HEAP_MAGIC;
    heap_head->size = heap_size - HEADER_SIZE;
    heap_head->free = 1;
    heap_head->next = NULL;
    heap_head->prev = NULL;
}

void *kmalloc(size_t size) {
    if (!heap_head) heap_init();
    if (size == 0) return NULL;

    size_t aligned_size = ALIGN8(size);
    block_header_t *curr = heap_head;

    while (curr) {
        if (curr->free && curr->size >= aligned_size) {
            size_t leftover = curr->size - aligned_size;

            // Only split if leftover can hold header + MIN_BLOCK_SIZE
            if (leftover >= HEADER_SIZE + MIN_BLOCK_SIZE) {
                block_header_t *new_block = (block_header_t *)((uintptr_t)curr + HEADER_SIZE + aligned_size);
                new_block->magic = HEAP_MAGIC;
                new_block->size = leftover - HEADER_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;
                new_block->prev = curr;

                if (curr->next) curr->next->prev = new_block;
                curr->next = new_block;
                curr->size = aligned_size;
            }

            curr->free = 0;
            heap_validate();
            return (void *)(curr + 1);
        }
        curr = curr->next;
    }

    // Out of memory
    return NULL;
}

void kfree(void *ptr) {
    if (!ptr) return;

    block_header_t *block = (block_header_t *)ptr - 1;

    HEAP_ASSERT(
        block->magic == HEAP_MAGIC,
        "heap: invalid or corrupted block header in free",
        PANIC_HEAP_MAGIC_CORRUPTION
    );

    if (block->free) {
        panic(
            "heap: double free detected",
            PANIC_HEAP_DOUBLE_FREE
        );
    }

    block->free = 1;

    // Coalesce next
    if (block->next && block->next->free) {
        block_header_t *next = block->next;
        block->size += HEADER_SIZE + next->size;
        block->next = next->next;
        if (next->next) next->next->prev = block;
    }

    // Coalesce previous
    if (block->prev && block->prev->free) {
        block_header_t *prev = block->prev;
        prev->size += HEADER_SIZE + block->size;
        prev->next = block->next;
        if (block->next) block->next->prev = prev;
        block = prev;
    }

    heap_validate();
}

// Aligned allocation
void *kmalloc_a(size_t size, size_t align) {
    uintptr_t raw = (uintptr_t)kmalloc(size + align + sizeof(uintptr_t));
    if (!raw) return NULL;

    uintptr_t aligned = (raw + sizeof(uintptr_t) + (align - 1)) & ~(align - 1);
    ((uintptr_t*)aligned)[-1] = raw;  // store original pointer
    return (void*)aligned;
}

void kfree_a(void *ptr) {
    if (!ptr) return;
    uintptr_t raw = ((uintptr_t*)ptr)[-1];
    kfree((void*)raw);
}