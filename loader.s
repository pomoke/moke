
.set MAGIC_NUMBER, 0x1BADB002
.set FLAGS, 0x0
.set CHECKSUM, -MAGIC_NUMBER
.set KERNEL_STACK_SIZE, 8192

.text
	.globl loader
	.align 4
	.long MAGIC_NUMBER
	.long FLAGS
	.long CHECKSUM

	loader:
		movl $(kernel_stack+KERNEL_STACK_SIZE),%esp /*set up kernel stack*/
		movl %eax,(boottype)

	jmp spin_up

loop:
	jmp loop

.bss
	.align 4
	.globl boottype
	.comm kernel_stack,KERNEL_STACK_SIZE
	.comm boottype,4
