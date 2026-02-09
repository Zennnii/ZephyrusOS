#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "stdint.h"

/* --- Multiboot2 Tag Header --- */
struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

/* --- Framebuffer Tag (type = 8) --- */
struct multiboot_tag_framebuffer {
    uint32_t type;
    uint32_t size;

    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;

    // if fb_type == 1 (RGB), more fields follow (RGB masks), but optional
};

/* --- Memory Map Tag (type = 6) --- */
struct multiboot_tag_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;

    struct multiboot_mmap_entry {
        uint64_t addr;   // base address
        uint64_t len;    // length
        uint32_t type;   // 1 = usable, others = reserved
        uint32_t zero;   // reserved
    } entries[0];
};

/* --- Module Tag (type = 3) --- */
struct multiboot_tag_module {
    uint32_t type;
    uint32_t size;
    uint32_t mod_start;  // physical address where module starts
    uint32_t mod_end;    // physical address where module ends
    char string[0];       // optional name or description (null-terminated)
};

/* --- Useful Multiboot2 Tag Types --- */
#define MULTIBOOT_TAG_TYPE_END           0
#define MULTIBOOT_TAG_TYPE_CMDLINE       1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER   2
#define MULTIBOOT_TAG_TYPE_MODULE        3
#define MULTIBOOT_TAG_TYPE_MMAP          6
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER   8

#endif