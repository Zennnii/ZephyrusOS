#include "CPU/GDT/gdt.h"
#include "CPU/IDT/idt.h"
#include "CPU/IDT/interrupts.h"
#include "CPU/IDT/IRQ/irq.h"
#include "CPU/IDT/isr.h"
#include "init.h"
#include "drivers/PIT/pit.h"
#include "drivers/PS2_Keyboard_Driver/keyboard.h"
#include "drivers/cmos_rtc/cmos_rtc.h"
#include "drivers/Speaker/speaker.h"
#include "drivers/ata/ata.h"
#include "stdint.h"
#include "debug/debug_tools.h"
#include "CLI/cli.h"
#include "multiboot.h"
#include "fs/fat16/fat16.h"
#include "gfx.h"
#include "ramfs/ramfs.h"

//uint32_t fb_pitch = 0;
//uint32_t fb_pitch_pixels = 0;
//uint32_t *pixels = NULL;

static inline uint32_t align8(uint32_t v) { return (v + 7) & ~7; }

void kmain(uint32_t multiboot_info_addr) {

    // Ensure interrupts are disabled during critical setup
    __asm__ volatile("cli");

    init_fb(multiboot_info_addr);
    init_ramfs(multiboot_info_addr);

    // Phase 1: Core CPU Setup (interrupts disabled)
    newLineFB();
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

    LOG_LOAD("Detecting if there is a drive present...");
    ata_init();

    LOG_LOAD("Initializing FAT16...");
    fat16_init();

    // Phase 5: System ready
    newLine();
    LOG_INFO("Kernel initialization complete");
    LOG_INFO("Loading kernel CLI...");

    int BSx = (fb_width  - 362) / 2;
    int BSy = (fb_height - 354) / 2;

    uint32_t bs_size;
    uint8_t *bs_data = ramfs_get("BootSplash.sri", &bs_size);

    if (bs_data) {
    draw_sri(bs_data, 331, 207, 0x00000000); // center it and draw
    } 
    else {
        LOG_ERR("BootSplash.sri not found in RAMFS!");
    }

    // Brief pause before starting CLI
    wait(3);
    Reset();

    clear(0x637a87);

    curLine = 0; // reset line counter at boot

        draw_string(fb, fb_width, 0, 0, "Zephyrus OS\n", 0xFFFFFFFF);
        draw_string(fb, fb_width, 0, 0, "Version 1.3.0\n", 0xFFFFFFFF);

        //draw_rect(50, 100, 200, 200, 0xFFFFFFFF);
        //draw_rect_filled(50, 10, 50, 50, 0xFFFFFFFF);
        //draw_triangle(100, 50, 50, 136, 150, 136, 0xFFFFFFFF);
        newLineFB();

        if (!ata_drive_present) {
            draw_string(fb, fb_width, 0, curLine, "No FAT16 drive was detected. The OS is running in live mode.\n", colorWhite);
            newLineFB();
        }

    // Phase 6: Main kernel loop
    cli();

    while (1) {
        check_exceptions();
        __asm__ volatile("hlt"); 
    }
}