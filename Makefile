CC = gcc
OBJECTS = system.o loader.o vga.o gdt.o mb.o interrupt.o
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -c -g -mgeneral-regs-only -mno-red-zone 
LDFLAGS = -Tlink.ld -melf_i386
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

