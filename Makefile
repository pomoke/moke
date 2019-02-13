CC = gcc
OBJECTS = system.o loader.o vga.o
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -c -g
LDFLAGS = -Tlink.ld -melf_i386
AS = as
ASFLAGS = --32 -gstabs

.PHONY : clean

all : kernel

kernel : $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel

strip :
	strip kernel -o s_kernel

%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

cdrom : kernel
	cp kernel iso/boot/kernel
	genisoimage -R \
		-b boot/grub/stage2 \
		-no-emul-boot \
		-boot-load-size 4 \
		-A kernel \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso 
qemu : cdrom
	qemu-system-x86_64 -m 128M -cdrom os.iso

qemu-dbg : cdrom 
	qemu-system-x86_64 -m 128M -kernel kernel -S -gdb tcp::1234

clean : 
	rm -rf *.o iso/boot/kernel *.iso kernel s_kernel

