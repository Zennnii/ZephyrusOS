#ifndef INIT_H
#define INIT_H

#include "stdint.h"
#include "string.h"

void init_fb(uint32_t multiboot_info_addr);
void init_ramfs(uint32_t multiboot_info_addr);

typedef struct {
    const char *name;
    uint8_t *start;
    uint32_t size;
} ramfs_file_t;


/* Global RAMFS info */
#define MAX_RAMFS_FILES 16
extern ramfs_file_t ramfs_files[MAX_RAMFS_FILES];
extern uint32_t ramfs_file_count;
/*-------------------*/

extern uint64_t total_ram;
void parse_mmap(void *mb2_addr);

#endif
