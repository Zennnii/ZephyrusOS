all: clean kernel.o vga.o gdt.o boot.o gdts.o image

clean: 
	rm -rf *.o

kernel.o: src/kernel.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/kernel.c -o kernel.o

vga.o: src/vga.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/vga.c -o vga.o

gdt.o: src/gdt.c
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -c src/gdt.c -o gdt.o

boot.o: src/boot.s
	nasm -f elf32 src/boot.s -o boot.o

gdts.o: src/gdt.s
	nasm -f elf32 src/gdt.s -o gdts.o

image: kernel.o vga.o gdt.o boot.o gdts.o
	ld -m elf_i386 -T linker.ld boot.o kernel.o vga.o gdt.o gdts.o -o kernel
	mkdir -p Zephyrus/boot
	cp kernel Zephyrus/boot/kernel
	grub-mkrescue -o Zephyrus.iso Zephyrus

riso:
	qemu-system-i386 Zephyrus.iso