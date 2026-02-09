#include "init.h"
#include "multiboot.h"
#include "stdint.h"
#include "gfx.h"
#include "debug/debug_tools.h"

/* Framebuffer globals — already defined elsewhere */
extern uint32_t *fb;
extern uint32_t fb_width;
extern uint32_t fb_height;
extern uint32_t fb_pitch;
extern uint32_t fb_pitch_pixels;

/* RAMFS globals */
#define MAX_RAMFS_FILES 16

ramfs_file_t ramfs_files[MAX_RAMFS_FILES];
uint32_t ramfs_file_count = 0;

static inline uint32_t align8(uint32_t v) { return (v + 7) & ~7; }

/* Initialize framebuffer */
void init_fb(uint32_t multiboot_info_addr) {
    struct multiboot_tag *tag;
    struct multiboot_tag_framebuffer *fb_tag = NULL;

    for (tag = (struct multiboot_tag*)((uint8_t*)multiboot_info_addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag*)((uint8_t*)tag + align8(tag->size))) {

        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) { 
            fb_tag = (struct multiboot_tag_framebuffer*)tag;

            fb = (uint32_t*)(uintptr_t)fb_tag->addr;
            fb_width = fb_tag->width;
            fb_height = fb_tag->height;
            fb_pitch = fb_tag->pitch;
            fb_pitch_pixels = fb_tag->pitch / 4;

            // Clear screen to black
            for (uint32_t y = 0; y < fb_height; y++) {
                for (uint32_t x = 0; x < fb_width; x++) {
                    fb[y * fb_pitch_pixels + x] = 0x00000000;
                }
            }

            LOG_LOAD("Framebuffer initialized");
            return;
        }
    }

}

/* Initialize RAMFS from GRUB modules */
void init_ramfs(uint32_t multiboot_info_addr) {
    struct multiboot_tag *tag;

    LOG_INFO("Scanning multiboot modules...");

    for (tag = (struct multiboot_tag*)((uint8_t*)multiboot_info_addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag*)((uint8_t*)tag + align8(tag->size))) {

        if (tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            struct multiboot_tag_module *mod = (struct multiboot_tag_module*)tag;

            if (ramfs_file_count < MAX_RAMFS_FILES) {
                static char name_buffer[MAX_RAMFS_FILES][64]; // store copies
                strncpy(name_buffer[ramfs_file_count], mod->string, 63);
                name_buffer[ramfs_file_count][63] = '\0'; // ensure null-termination

                ramfs_files[ramfs_file_count].name  = name_buffer[ramfs_file_count];
                ramfs_files[ramfs_file_count].start = (uint8_t*)(uintptr_t)mod->mod_start;
                ramfs_files[ramfs_file_count].size  = mod->mod_end - mod->mod_start;
                ramfs_file_count++;

                LOG_LOAD("Module loaded:");
                //LOG_INFO(ramfs_files[ramfs_file_count-1].name);
            }
        }
    }

    if (ramfs_file_count == 0)
        LOG_ERR("No RAMFS modules found!");
    else
        LOG_INFO("RAMFS initialized");
}

uint64_t total_ram = 0;

void parse_mmap(void *mb2_addr) {
    uint8_t *ptr = (uint8_t *)mb2_addr + 8;

    while (1) {
        struct multiboot_tag *tag = (struct multiboot_tag *)ptr;

        if (tag->type == 0)
            break;

        if (tag->type == 6) { // mmap
            struct multiboot_tag_mmap *mmap = (void *)tag;

            uint8_t *entry_ptr = ptr + sizeof(struct multiboot_tag_mmap);
            uint8_t *end = ptr + tag->size;

            while (entry_ptr < end) {
                struct multiboot_mmap_entry *entry =
                    (struct multiboot_mmap_entry *)entry_ptr;

                if (entry->type == 1) {
                    total_ram += entry->len;
                }

                entry_ptr += mmap->entry_size;
            }
        }

        ptr += (tag->size + 7) & ~7;
    }
}