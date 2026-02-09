.PHONY: all clean riso rundebug

clean:
	rm -rf *.o kernel_elf Zephyrus.iso

CFLAGS = -m32 -ffreestanding -fno-builtin -nostdlib -nostdinc \
         -Iinclude -Isrc/kernel -Isrc/libs -Isrc/includes -I src/kernel/graphics \
		 -fno-asynchronous-unwind-tables -fno-exceptions -fno-unwind-tables

OBJS = kernel.o vga.o util.o gdt.o idt.o isr.o irq.o pit.o keyboard.o kmalloc.o \
       boot.o gdts.o idts.o irqs.o init.o debug_tools.o cli.o string.o bcmds.o logf.o \
       cmos_rtc.o bmcli.o bmcmds.o speaker.o ata.o fat16.o ramfs.o dis.o colors.o panic.o \
	   text.o line.o rectangle.o gfx.o triangle.o sri.o com_1.o

all: $(OBJS) image

kernel.o: src/kernel/kernel.c
	gcc  $(CFLAGS) -c src/kernel/kernel.c -o kernel.o

vga.o: src/kernel/vga.c
	gcc  $(CFLAGS) -c src/kernel/vga.c -o vga.o

util.o: src/kernel/util/util.c
	gcc  $(CFLAGS) -c src/kernel/util/util.c -o util.o

gdt.o: src/kernel/CPU/GDT/gdt.c
	gcc $(CFLAGS) -c src/kernel/CPU/GDT/gdt.c -o gdt.o

idt.o: src/kernel/CPU/IDT/idt.c
	gcc  $(CFLAGS) -c src/kernel/CPU/IDT/idt.c -o idt.o

isr.o: src/kernel/CPU/IDT/isr.c
	gcc  $(CFLAGS) -c src/kernel/CPU/IDT/isr.c -o isr.o

irq.o: src/kernel/CPU/IDT/IRQ/irq.c
	gcc  $(CFLAGS) -c src/kernel/CPU/IDT/IRQ/irq.c -o irq.o

init.o: src/kernel/init.c
	gcc $(CFLAGS) -c src/kernel/init.c -o init.o

pit.o: src/kernel/drivers/PIT/pit.c
	gcc  $(CFLAGS) -c src/kernel/drivers/PIT/pit.c -o pit.o

keyboard.o: src/kernel/drivers/PS2_Keyboard_Driver/keyboard.c
	gcc $(CFLAGS) -c src/kernel/drivers/PS2_Keyboard_Driver/keyboard.c -o keyboard.o

kmalloc.o: src/kernel/mm/kmalloc/kmalloc.c
	gcc $(CFLAGS) -c src/kernel/mm/kmalloc/kmalloc.c -o kmalloc.o

boot.o: src/boot/boot.s
	nasm -f elf32 src/boot/boot.s -o boot.o

gdts.o: src/kernel/CPU/GDT/gdt.s
	nasm -f elf32 src/kernel/CPU/GDT/gdt.s -o gdts.o

idts.o: src/kernel/CPU/IDT/idts.s
	nasm -f elf32 src/kernel/CPU/IDT/idts.s -o idts.o

irqs.o: src/kernel/CPU/IDT/IRQ/irqs.s
	nasm -f elf32 src/kernel/CPU/IDT/IRQ/irqs.s -o irqs.o

debug_tools.o: src/kernel/debug/debug_tools.c
	gcc $(CFLAGS) -c src/kernel/debug/debug_tools.c -o debug_tools.o

cli.o: src/kernel/CLI/cli.c
	gcc $(CFLAGS) -c src/kernel/CLI/cli.c -o cli.o

string.o: src/libs/string.c
	gcc $(CFLAGS) -c src/libs/string.c -o string.o

bcmds.o: src/kernel/CLI/bcmds/bcmds.c
	gcc $(CFLAGS) -c src/kernel/CLI/bcmds/bcmds.c -o bcmds.o

logf.o: src/kernel/logf/logf.c
	gcc $(CFLAGS) -c src/kernel/logf/logf.c -o logf.o

cmos_rtc.o: src/kernel/drivers/cmos_rtc/cmos_rtc.c
	gcc $(CFLAGS) -c src/kernel/drivers/cmos_rtc/cmos_rtc.c -o cmos_rtc.o

bmcli.o: src/kernel/CLI/bmcli/bmcli.c
	gcc $(CFLAGS) -c src/kernel/CLI/bmcli/bmcli.c -o bmcli.o

bmcmds.o: src/kernel/CLI/bmcli/bmcmds/bmcmds.c
	gcc $(CFLAGS) -c src/kernel/CLI/bmcli/bmcmds/bmcmds.c -o bmcmds.o

pcid.o: src/kernel/CLI/pcid_cmds/pcid.c
	gcc $(CFLAGS) -c src/kernel/CLI/pcid_cmds/pcid.c -o pcid.o

speaker.o: src/kernel/drivers/Speaker/speaker.c
	gcc $(CFLAGS) -c src/kernel/drivers/Speaker/speaker.c -o speaker.o

ata.o: src/kernel/drivers/ata/ata.c
	gcc $(CFLAGS) -c src/kernel/drivers/ata/ata.c -o ata.o

com_1.o: src/kernel/drivers/com_1/com_1.c
	gcc $(CFLAGS) -c src/kernel/drivers/com_1/com_1.c -o com_1.o

fat16.o: src/kernel/fs/fat16/fat16.c
	gcc $(CFLAGS) -c src/kernel/fs/fat16/fat16.c -o fat16.o

ramfs.o: src/kernel/ramfs/ramfs.c
	gcc $(CFLAGS) -c src/kernel/ramfs/ramfs.c -o ramfs.o
	
dis.o: src/kernel/graphics/drawing/dis.c
	gcc $(CFLAGS) -c src/kernel/graphics/drawing/dis.c -o dis.o

text.o: src/kernel/graphics/text/text.c
	gcc $(CFLAGS) -c src/kernel/graphics/text/text.c -o text.o

gfx.o: src/kernel/graphics/gfx.c
	gcc $(CFLAGS) -c src/kernel/graphics/gfx.c -o gfx.o

colors.o: src/kernel/graphics/colors.c
	gcc $(CFLAGS) -c src/kernel/graphics/colors.c -o colors.o

line.o: src/kernel/graphics/drawing/line/line.c
	gcc $(CFLAGS) -c src/kernel/graphics/drawing/line/line.c -o line.o

rectangle.o: src/kernel/graphics/drawing/shapes/rectangle/rectangle.c
	gcc $(CFLAGS) -c src/kernel/graphics/drawing/shapes/rectangle/rectangle.c -o rectangle.o

triangle.o: src/kernel/graphics/drawing/shapes/triangle/triangle.c
	gcc $(CFLAGS) -c src/kernel/graphics/drawing/shapes/triangle/triangle.c -o triangle.o

sri.o: src/kernel/graphics/drawing/images/sri.c
	gcc $(CFLAGS) -c src/kernel/graphics/drawing/images/sri.c -o sri.o

panic.o: src/kernel/panic/panic.c
	gcc $(CFLAGS) -c src/kernel/panic/panic.c -o panic.o

image: $(OBJS)
	ld -m elf_i386 -T linker.ld boot.o $(filter-out boot.o,$(OBJS)) -o kernel_elf
	mkdir -p Zephyrus/boot/grub
	cp kernel_elf Zephyrus/boot/kernel_elf
	grub-mkrescue -o Zephyrus.iso Zephyrus

riso:
	qemu-system-i386 -cdrom Zephyrus.iso \
	-hda disk.img \
    -boot d \
    -m 2G \
    -audiodev pa,id=snd \
    -machine pcspk-audiodev=snd \
	-serial file:BOOTLOG

riso-nodrive:
	qemu-system-i386 -cdrom Zephyrus.iso \
    -m 2G \
    -audiodev pa,id=snd \
    -machine pcspk-audiodev=snd \
	-serial file:BOOTLOG

run-debug:
	qemu-system-i386 -cpu qemu32 -monitor stdio -d in_asm,cpu,int,guest_errors -D qemu.log -no-reboot -cdrom Zephyrus.iso \
    -hda disk.img \
    -boot d \
    -m 2G \
    -audiodev pa,id=snd \
    -machine pcspk-audiodev=snd
