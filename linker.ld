OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
    /* Load address for the kernel */
    . = 0x00100000;

    /* Code section: Read + Execute */
    .text ALIGN(4K) : AT(ADDR(.text)) {
        *(.multiboot)
        *(.text)
        *(.rodata*)
    }

    /* Data section: Read + Write */
    .data ALIGN(4K) : AT(ADDR(.data)) {
        *(.data)
    }

    /* BSS section: Read + Write, zeroed */
    .bss ALIGN(4K) : AT(ADDR(.bss)) {
        *(COMMON)
        *(.bss)
    }
}