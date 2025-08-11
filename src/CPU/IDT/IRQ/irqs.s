%macro IRQ_STUB 1
global irq%1
extern irq%1_handler

irq%1:
    cli
    pushad
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword 0
    push dword 32 + %1
    call irq%1_handler
    add esp, 8

    pop gs
    pop fs
    pop es
    pop ds
    popad
    sti

    mov al, 0x20
    out 0x20, al
    iret
%endmacro

IRQ_STUB 0
IRQ_STUB 1
;IRQ_STUB 2
;IRQ_STUB 3
;IRQ_STUB 4
;IRQ_STUB 5
;IRQ_STUB 6
;IRQ_STUB 7
;IRQ_STUB 8
;IRQ_STUB 9
;IRQ_STUB 10
;IRQ_STUB 11
;IRQ_STUB 12
;IRQ_STUB 13
;IRQ_STUB 14
;IRQ_STUB 15

section .note.GNU-stack noalloc noexec nowrite