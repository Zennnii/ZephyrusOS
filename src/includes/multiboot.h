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

    uint64_t addr;       // framebuffer address
    uint32_t pitch;      // bytes per scanline
    uint32_t widthfb;
    uint32_t heightfb;
    uint8_t  bpp;
    uint8_t  fb_type;    // 0 = indexed, 1 = RGB, 2 = text
    uint16_t reserved;

    // if fb_type == 1 (RGB), more fields follow (RGB masks), but optional
};

/* --- Memory Map Tag (type = 6) --- */
struct multiboot_tag_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;

    /* Followed by entries[] */
    struct multiboot_mmap_entry {
        uint64_t addr;   // base address
        uint64_t len;    // length
        uint32_t type;   // 1 = usable, others = reserved
        uint32_t zero;   // reserved
    } entries[0];        // flexible array
};

#endif