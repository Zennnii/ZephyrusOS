all: clean kernel.o vga.o util.o gdt.o idt.o isr.o irq.o pit.o keyboard.o kmalloc.o boot.o gdts.o idts.o irqs.o debug_tools.o image

clean: 
	rm -rf *.o

kernel.o: src/kernel.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/kernel.c -o kernel.o

vga.o: src/vga.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/vga.c -o vga.o

util.o: src/util.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/util.c -o util.o

gdt.o: src/CPU/GDT/gdt.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/CPU/GDT/gdt.c -o gdt.o

idt.o: src/CPU/IDT/idt.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/CPU/IDT/idt.c -o idt.o

isr.o: src/CPU/IDT/isr.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/CPU/IDT/isr.c -o isr.o

irq.o: src/CPU/IDT/IRQ/irq.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/CPU/IDT/IRQ/irq.c -o irq.o

pit.o: src/drivers/PIT/pit.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/drivers/PIT/pit.c -o pit.o

keyboard.o: src/drivers/PS2_Keyboard_Driver/keyboard.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/drivers/PS2_Keyboard_Driver/keyboard.c -o keyboard.o

kmalloc.o: src/mm/kmalloc/kmalloc.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/mm/kmalloc/kmalloc.c -o kmalloc.o

boot.o: src/boot.s
	nasm -f elf32 src/boot.s -o boot.o

gdts.o: src/CPU/GDT/gdt.s
	nasm -f elf32 src/CPU/GDT/gdt.s -o gdts.o

idts.o: src/CPU/IDT/idts.s
	nasm -f elf32 src/CPU/idt/idts.s -o idts.o

irqs.o: src/CPU/IDT/IRQ/irqs.s
	nasm -f elf32 src/CPU/IDT/IRQ/irqs.s -o irqs.o

debug_tools.o:
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/debug_tools.c -o debug_tools.o


image: kernel.o vga.o gdt.o boot.o gdts.o
	ld -m elf_i386 -T linker.ld boot.o kernel.o vga.o util.o debug_tools.o gdt.o gdts.o idt.o idts.o isr.o irq.o irqs.o pit.o keyboard.o kmalloc.o -o kernel
	mkdir -p Zephyrus/boot
	cp kernel Zephyrus/boot/kernel
	grub-mkrescue -o Zephyrus.iso Zephyrus

riso:
	qemu-system-i386 Zephyrus.iso

rundebug:
	qemu-system-i386 -monitor stdio -d int,cpu_reset -D qemu.log -no-reboot Zephyrus.iso