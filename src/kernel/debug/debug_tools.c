#include "debug_tools.h"
#include "CPU/IDT/interrupts.h"
#include "mm/kmalloc/kmalloc.h"
#include "vga.h"

void print_current_cpu_state() {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp;
    uint32_t eip, eflags;
    uint16_t cs, ds, es, fs, gs, ss;
    
    // Get general purpose registers
    asm volatile ("mov %%eax, %0" : "=m"(eax));
    asm volatile ("mov %%ebx, %0" : "=m"(ebx));
    asm volatile ("mov %%ecx, %0" : "=m"(ecx));
    asm volatile ("mov %%edx, %0" : "=m"(edx));
    asm volatile ("mov %%esi, %0" : "=m"(esi));
    asm volatile ("mov %%edi, %0" : "=m"(edi));
    asm volatile ("mov %%esp, %0" : "=m"(esp));
    asm volatile ("mov %%ebp, %0" : "=m"(ebp));
    
    // Get segment registers
    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("mov %%ds, %0" : "=r"(ds));
    asm volatile ("mov %%es, %0" : "=r"(es));
    asm volatile ("mov %%fs, %0" : "=r"(fs));
    asm volatile ("mov %%gs, %0" : "=r"(gs));
    asm volatile ("mov %%ss, %0" : "=r"(ss));
    
    // Get flags register
    asm volatile ("pushf; pop %0" : "=r"(eflags));
    
    // Get current instruction pointer (approximately)
    asm volatile ("call 1f; 1: pop %0" : "=r"(eip));
    
    draw_string(fb, fb_width, curX, curLine, "=== CURRENT CPU STATE ===\n", colorWhite);
    draw_string(fb, fb_width, curX, curLine, "EAX: ", colorWhite); draw_hex(eax); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "EBX: ", colorWhite); draw_hex(ebx); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "ECX: ", colorWhite); draw_hex(ecx); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "EDX: ", colorWhite); draw_dec(edx); newLineFB();
    
    draw_string(fb, fb_width, curX, curLine, "ESI: ", colorWhite); draw_hex(esi); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "EDI: ", colorWhite); draw_hex(edi); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "ESP: ", colorWhite); draw_hex(esp); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "EBP: ", colorWhite); draw_hex(ebp); newLineFB;
    
    draw_string(fb, fb_width, curX, curLine, "EIP: ", colorWhite); draw_hex(eip); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "EFLAGS: ", colorWhite); draw_hex(eflags); newLineFB;
    
    draw_string(fb, fb_width, curX, curLine, "CS: ", colorWhite); draw_hex(cs); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "DS: ", colorWhite); draw_hex(ds); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "ES: ", colorWhite); draw_hex(es); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "SS: ", colorWhite); draw_hex(ss); newLineFB;
    newLineFB();
}

void print_segment_registers() {
    uint16_t cs, ds, es, fs, gs, ss;

    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("mov %%ds, %0" : "=r"(ds));
    asm volatile ("mov %%es, %0" : "=r"(es));
    asm volatile ("mov %%fs, %0" : "=r"(fs));
    asm volatile ("mov %%gs, %0" : "=r"(gs));
    asm volatile ("mov %%ss, %0" : "=r"(ss));

    draw_string(fb, fb_width, curX, curLine, "Segment registers:\n", colorWhite);
    draw_string(fb, fb_width, curX, curLine, "CS = 0x", colorWhite); draw_hex(cs); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "DS = 0x", colorWhite); draw_hex(ds); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "ES = 0x", colorWhite); draw_hex(es); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "FS = 0x", colorWhite); draw_hex(fs); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "GS = 0x", colorWhite); draw_hex(gs); newLineFB();
    draw_string(fb, fb_width, curX, curLine, "SS = 0x", colorWhite); draw_hex(ss); newLineFB();
    newLineFB();
}

void test_kmalloc() {
    draw_string(fb, fb_width, curX, curLine, "===== Testing kmalloc/kfree =====\n", colorWhite);

    // Allocate a small block
    void* a = kmalloc(16);
    draw_string(fb, fb_width, curX, curLine, "Allocated a: ", colorWhite); draw_hex((uintptr_t)a); newLineFB();

    // Allocate a large block
    void* b = kmalloc(64);
    draw_string(fb, fb_width, curX, curLine, "Allocated b: ", colorWhite); draw_hex((uintptr_t)b); newLineFB();

    // Free the first block
    kfree(a);
    draw_string(fb, fb_width, curX, curLine, "Freed a\n", colorWhite);

    // Allocate again — should reuse freed block if possible
    void* c = kmalloc(8);
    draw_string(fb, fb_width, curX, curLine, "Allocated c: ", colorWhite); draw_hex((uintptr_t)c); newLineFB();

    // Check if c == a
    if (c == a) {
        draw_string(fb, fb_width, curX, curLine, "Memory reuse works\n", colorWhite);
    }
    else {
        draw_string(fb, fb_width, curX, curLine, "Memory reuse failed\n", colorWhite);
    }

    int count = 0;
    while (1) {
        void *ptr = kmalloc(128);
        if (!ptr) {
            draw_string(fb, fb_width, curX, curLine, "Heap exhausted after: ", colorWhite);
            print_dec(count);
            draw_string(fb, fb_width, curX, curLine, " allocations.\n", colorWhite);
            break;
        }
        count++;
    }

    draw_string(fb, fb_width, curX, curLine, "===== Test complete =====\n", colorWhite);
}