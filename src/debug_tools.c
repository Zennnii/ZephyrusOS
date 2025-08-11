#include "debug_tools.h"
#include "CPU/IDT/interrupts.h"
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
    
    print("=== CURRENT CPU STATE ===\n");
    print("EAX: "); print_hex(eax);
    print(" EBX: "); print_hex(ebx);
    print(" ECX: "); print_hex(ecx);
    print(" EDX: "); print_hex(edx); print("\n");
    
    print("ESI: "); print_hex(esi);
    print(" EDI: "); print_hex(edi);
    print(" ESP: "); print_hex(esp);
    print(" EBP: "); print_hex(ebp); print("\n");
    
    print("EIP: "); print_hex(eip);
    print(" EFLAGS: "); print_hex(eflags); print("\n");
    
    print("CS: "); print_hex(cs);
    print(" DS: "); print_hex(ds);
    print(" ES: "); print_hex(es);
    print(" SS: "); print_hex(ss); print("\n");
    newLine();
}

void print_segment_registers() {
    uint16_t cs, ds, es, fs, gs, ss;

    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("mov %%ds, %0" : "=r"(ds));
    asm volatile ("mov %%es, %0" : "=r"(es));
    asm volatile ("mov %%fs, %0" : "=r"(fs));
    asm volatile ("mov %%gs, %0" : "=r"(gs));
    asm volatile ("mov %%ss, %0" : "=r"(ss));

    print("Segment registers:\n");
    print("CS = 0x"); print_hex(cs ); print(" ");
    print("DS = 0x"); print_hex(ds ); print(" ");
    print("ES = 0x"); print_hex(es ); print("\n");
    print("FS = 0x"); print_hex(fs ); print(" ");
    print("GS = 0x"); print_hex(gs ); print(" ");
    print("SS = 0x"); print_hex(ss ); print("\n");
    newLine();
}