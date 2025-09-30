SECTIONS
{
    . = 0x00100000;

    .multiboot2 ALIGN(8) : {
        *(.multiboot2)
    }

    .text ALIGN(4K) : {
        *(.text)
        *(.rodata*)
    }

    .data ALIGN(4K) : {
        *(.data)
    }

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }
}
