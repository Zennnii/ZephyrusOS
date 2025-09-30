#include "CPU/GDT/gdt.h"
#include "CPU/IDT/idt.h"
#include "CPU/IDT/interrupts.h"
#include "CPU/IDT/IRQ/irq.h"
#include "CPU/IDT/isr.h"
#include "drivers/PIT/pit.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "drivers/cmos_rtc/cmos_rtc.h"
#include "drivers/Speaker/speaker.h"
#include "drivers/ata/ata.h"
#include "vga.h"
#include "stdint.h"
#include "debug/debug_tools.h"
#include "CLI/cli.h"
#include "multiboot.h"
#include "fs/fat16/fat16.h"
#include "fb/dis.h"

uint32_t fb_pitch = 0;
uint32_t fb_pitch_pixels = 0;
uint32_t *pixels = NULL;

static inline uint32_t align8(uint32_t v) { return (v + 7) & ~7; }

void kmain(uint32_t multiboot_info_addr) {

    // Ensure interrupts are disabled during critical setup
    __asm__ volatile("cli");
    
    struct multiboot_tag *tag;
    uint8_t *addr = (uint8_t*)multiboot_info_addr;

    // First pass: find framebuffer and set globals
    struct multiboot_tag_framebuffer *fb_tag = NULL;
    for (tag = (struct multiboot_tag*)(addr + 8);
         tag->type != 0;
         tag = (struct multiboot_tag*)((uint8_t*)tag + align8(tag->size))) {

        switch (tag->type) {
        case 6: { // memory map
            struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap*)tag;
            uint32_t count = (mmap->size - sizeof(*mmap)) / mmap->entry_size;

            for (uint32_t i = 0; i < count; i++) {
                struct multiboot_mmap_entry *e =
                    (struct multiboot_mmap_entry*)((uint8_t*)mmap->entries + i * mmap->entry_size);

                // Example: log usable regions
                if (e->type == 1) {
                    // available RAM
                    // e->addr = base, e->len = length
                }
            }
            break;
        }
        case 8: { // framebuffer
            fb_tag = (struct multiboot_tag_framebuffer*)tag;
            
            // Set up global framebuffer variables
            fb = (uint32_t*)(uintptr_t)fb_tag->addr;
            fb_width = fb_tag->widthfb;
            fb_height = fb_tag->heightfb;
            fb_pitch = fb_tag->pitch;
            fb_pitch_pixels = fb_tag->pitch / 4;
            pixels = fb;
            
            // Clear screen to black
            for (uint32_t y = 0; y < fb_height; y++) {
                for (uint32_t x = 0; x < fb_width; x++) {
                    pixels[y * fb_pitch_pixels + x] = 0x00000000;
                }
            }
            break;
        }
        }
    }


    // Phase 1: Core CPU Setup (interrupts disabled)
    LOG_LOAD("Initializing GDT...");
    initGdt();
    
    LOG_LOAD("Initializing IDT...");
    initIdt();

    LOG_LOAD("Installing ISRs...");
    isr_install();
    
    LOG_LOAD("Installing IRQs...");
    irq_install();
    
    // Phase 2: Memory Management (still interrupts disabled)
    LOG_LOAD("Initializing Heap...");
    heap_init();
    
    // Phase 3: Enable interrupts and test
    LOG_LOAD("Enabling Interrupts...");
    enableInterrupts();
    
    // Small delay to test if interrupts work without crashing
    for (volatile int i = 0; i < 1000000; i++);
    
    // Phase 4: Initialize hardware drivers (interrupts now enabled)
    LOG_LOAD("Initializing PIT...");
    pit_set_frequency(100);
    
    LOG_LOAD("Initializing PS/2 Keyboard Driver...");
    init_keyboard();
    
    LOG_LOAD("Initializing CMOS RTC...");
    init_rtc();

    LOG_LOAD("Initializing PC Speaker...");
    init_speaker();

    LOG_LOAD("Initializing FAT16...");
    fat16_init();

    // Phase 5: System ready
    newLine();
    LOG_INFO("Kernel initialization complete");
    LOG_INFO("Loading kernel CLI...");

    // Brief pause before starting CLI
    wait(2);
    Reset();

    for (uint32_t y = 0; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            pixels[y * fb_pitch_pixels + x] = 0x637a87;
        }
    }

    if (fb_tag) {
        curLine = 0; // reset line counter at boot

        draw_string(fb, fb_width, 0, 0, "Zephyrus OS\n", 0xFFFFFFFF);
        draw_string(fb, fb_width, 0, 0, "Version 1.2.0\n", 0xFFFFFFFF);
        draw_string(fb, fb_width, 0, curLine, "\n", 0xFF234FFF);
    }

    // Phase 6: Main kernel loop
    cli();

    wait(2);

    while (1) {
        check_exceptions();
        __asm__ volatile("hlt"); 
    }
}