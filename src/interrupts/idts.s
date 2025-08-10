global idt_flush

idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret

%macro ISR_NOERRORCODE 1
global isr%1
isr%1:
    cli
    push dword %1       ; Interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRORCODE 1
global isr%1
isr%1:
    cli
    push dword %1       ; Interrupt number
    ; CPU already pushed error code automatically
    jmp isr_common_stub
%endmacro

ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7

ISR_ERRORCODE 8
ISR_NOERRORCODE 9
ISR_ERRORCODE 10
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31

ISR_NOERRORCODE 128
ISR_NOERRORCODE 177

extern isr_handler

global isr_common_stub

isr_common_stub:
    pusha
    push ds

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp        ; Pointer to registers struct on stack
    push eax
    call isr_handler
    add esp, 4          ; Clean up argument

    pop ds
    popa

    ; Clean up pushed error code and interrupt number (2 dwords)
    add esp, 8

    sti
    iret

%macro IRQ 2
global irq%1
irq%1:
    cli
    push dword 0        ; Dummy error code
    push dword %2       ; IRQ number (32 + IRQ)
    jmp isr_common_stub
%endmacro

; Standard PC IRQs (0-15)
IRQ 0, 32    ; Timer (PIT)
IRQ 1, 33    ; Keyboard
IRQ 2, 34    ; Cascade (never raised)
IRQ 3, 35    ; COM2/COM4
IRQ 4, 36    ; COM1/COM3
IRQ 5, 37    ; LPT2
IRQ 6, 38    ; Floppy disk
IRQ 7, 39    ; LPT1
IRQ 8, 40    ; CMOS Real-time clock
IRQ 9, 41    ; Free for peripherals / legacy SCSI / NIC
IRQ 10, 42   ; Free for peripherals / SCSI / NIC
IRQ 11, 43   ; Free for peripherals / SCSI / NIC
IRQ 12, 44   ; PS/2 Mouse
IRQ 13, 45   ; FPU / Coprocessor / Inter-processor
IRQ 14, 46   ; Primary ATA Hard Disk
IRQ 15, 47   ; Secondary ATA Hard Disk