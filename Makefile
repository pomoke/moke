CC = gcc
SHELL = bash
OBJECTS = system.o loader_ng.o vga.o gdt.o mb.o interrupt.o cmos.o clock.o kprint.o power.o panic.o uart.o memory.o
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -c -g -mno-red-zone -fno-pic\
	 -isystem $(shell $(CC) -print-file-name=include) \
	 -isystem $(shell pwd)/header \
	 -mgeneral-regs-only
LDFLAGS = -Tlink_ng.ld -melf_i386
AS = as
ASFLAGS = --32 -gstabs

.PHONY : clean

all : kernel

kernel : $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o moke

strip :
	strip moke -o smoke

%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

cdrom : kernel
	cp moke iso/boot/moke
	genisoimage -R \
		-b boot/grub/stage2 \
		-no-emul-boot \
		-boot-load-size 4 \
		-A moke \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o moke.iso \
		iso/ 
qemu : kernel
	qemu-system-x86_64 -m 128M -kernel moke

qemu-cd: cdrom
	qemu-system-x86_64 -m 128M -cdrom moke.iso
cd-dbg: cdrom
	echo Debugger listening at tcp::1234
	qemu-system-x86_64 -m 512M -cdrom moke.iso -S -gdb tcp::1234
	#gdb -ex "set arch i386:x86-64" -ex "target remote tcp::1234" moke
qemu-dbg : kernel
	qemu-system-x86_64 -m 128M -kernel moke -S -gdb tcp::1234
kvm : kernel
	kvm -m 128M -kernel moke

clean : 
	rm -rf *.o iso/boot/kernel *.iso moke s_moke

