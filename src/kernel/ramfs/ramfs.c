#include "ramfs.h"
#include "init.h"
#include "string.h"

uint8_t* ramfs_get(const char *name, uint32_t *size_out) {
    for (uint32_t i = 0; i < ramfs_file_count; i++) {
        if (!strcmp(ramfs_files[i].name, name)) {
            if (size_out) *size_out = ramfs_files[i].size;
            return ramfs_files[i].start;
        }
    }
    return NULL;
}