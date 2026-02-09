; boot.s - Multiboot2 stub (with framebuffer + module support)
BITS 32

section .multiboot2
align 8

MB2_MAGIC    equ 0xE85250D6
MB2_ARCH     equ 0
MB2_LENGTH   equ header_end - header_start
MB2_CHECKSUM equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

header_start:
    dd MB2_MAGIC
    dd MB2_ARCH
    dd MB2_LENGTH
    dd MB2_CHECKSUM

    ; --- Framebuffer request (type 5) ---
    dw 5              ; type = 5
    dw 0              ; flags
    dd 20             ; size of this tag
    dd 1024           ; width
    dd 768            ; height
    dd 32             ; bits per pixel

    align 8

    ; --- End tag ---
    dw 0
    dw 0
    dd 8
header_end:

; Code
section .text
global start
extern kmain
global multiboot_info_addr  ; expose GRUB pointer for C code

start:
    mov esp, stack_top

    ; Store GRUB Multiboot2 info pointer
    mov [multiboot_info_addr], ebx

    push ebx
    call kmain

.hang:
    cli
    hlt
    jmp .hang

; Data section (for the global)
section .data
align 4
multiboot_info_addr: dd 0

; Stack
section .bss
align 16
stack_bottom:
    resb 16384         ; 16 KiB stack
stack_top:

section .note.GNU-stack noalloc noexec nowrite
