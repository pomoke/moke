CC = gcc
OBJECTS = system.o loader.o
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -c
LDFLAGS = -Tlink.ld -melf_i386
AS = as
ASFLAGS = --32

all : kernel

kernel : $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel

strip :
	strip kernel -o kernel.strip

%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

cdrom :
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
qemu :
	qemu-system-x86_64 -m 128M -cdrom os.iso

qemu-dbg :
	qemu-system-x86_64 -m 128M -cdrom os.iso -S -gdb tcp::1234
clean : 
	rm -rf *.o kernel

