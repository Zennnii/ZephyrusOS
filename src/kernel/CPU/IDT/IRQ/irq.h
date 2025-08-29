 #ifndef IRQ_H
 #define IRQ_H

 #include "stdint.h"
 #include "../idt.h"

struct interruptRegisters {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;  // placeholder for esp pushed by pushad
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t ds;
    uint32_t es;
    uint32_t fs;
    uint32_t gs;

    uint32_t int_no;
    uint32_t err_code;
};

// Function to initialize/remap PIC and set IRQ gates
void irq_install(void);
// Function to send EOI (End of interrput)
void send_eoi(uint8_t irq);

void irq0_handler();
void irq1_handler();

// IRQ handler function prototypes (implemented in assembly)
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif