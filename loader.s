.set MAGIC_NUMBER, 0x1BADB002
.set FLAGS, 0x3
.set CHECKSUM, -FLAGS-MAGIC_NUMBER
.set KERNEL_STACK_SIZE, 4096

.text
	.globl loader
	.align 4
	.long MAGIC_NUMBER
	.long FLAGS
	.long CHECKSUM

	loader:
		movl $(kernel_stack+KERNEL_STACK_SIZE),%esp /*set up kernel stack*/
		movl %eax,(boottype)
		movl %ebx,(mbinfo)

	call spin_up

loop:
	jmp loop

.globl finilize_gdt
finilize_gdt:
	//ljmp $0x08,$reload;
	mov $0x10,%ax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs
	mov $0x18,%ax
	mov %ax,%ss
	//mov %ax,%cs
	ljmp $0x08,$reload;
	reload:
	ret

.bss
	.align 4
	.globl boottype
	.globl mbinfo
	.globl kernel_stack
	.comm kernel_stack,KERNEL_STACK_SIZE
	.comm boottype,4
	.comm mbinfo,4
