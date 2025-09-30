;======================================================
; boot.s - Minimal GRUB Multiboot2 Bootstub
;======================================================
BITS 32

; ---------------------------
; Multiboot2 Header (must be in first 8 KiB)
; ---------------------------
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
    dd 32             ; bpp

    align 8

    ; --- End tag (type 0) ---
    dw 0
    dw 0
    dd 8
header_end:
; ---------------------------
; Code section
; ---------------------------
section .text
global start
extern kmain

start:
    ; Setup stack
    mov esp, stack_top

    ; Pass Multiboot2 info pointer to kernel
    push ebx

    ; Call kernel main
    call kmain

HaltKernel:
    cli
    hlt
    jmp HaltKernel

; ---------------------------
; BSS (stack)
; ---------------------------
section .bss
align 16
stack_bottom:
    resb 16384         ; 16 KiB stack
stack_top:

; ---------------------------
; Avoid GNU stack
; ---------------------------
section .note.GNU-stack noalloc noexec nowrite
