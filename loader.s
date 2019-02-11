
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
		movl $0xCAFEBABE,%eax
		movl $(kernel_stack+KERNEL_STACK_SIZE),%esp

	call spin_up
	pushl 3
	pushl 1
	pushl 2
	movw $0x4128,0x000b8000
	call sum
	call spin_up

.loop:
	jmp .loop

.bss
	.align 4
	.lcomm kernel_stack,KERNEL_STACK_SIZE
