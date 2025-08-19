BITS 32

; Multiboot header constants
MULTIBOOT_ALIGN    equ  1<<0             ; align loaded modules on page boundaries
MULTIBOOT_MEMINFO  equ  1<<1             ; provide memory map
MULTIBOOT_FLAGS    equ  MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO
MULTIBOOT_MAGIC    equ  0x1BADB002       ; multiboot magic number
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

section .multiboot
    align 4
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .bss
    align 16
stack_bottom:
    resb 16384          ; 16 KiB stack
stack_top:

section .text
global start
extern kmain

start:
    ; Set up the stack
    mov esp, stack_top
    
    ; Reset EFLAGS
    push 0
    popf
    
    ; Push multiboot info pointer (ebx contains it)
    push ebx
    
    ; Call the kernel main function
    call kmain
    
    ; If kmain returns, halt the system
    jmp HaltKernel

HaltKernel:
    cli
    hlt
    jmp HaltKernel

section .note.GNU-stack noalloc noexec nowrite