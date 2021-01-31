.set MAGIC_NUMBER, 0x1BADB002
.set FLAGS, 0x3
.set CHECKSUM, -FLAGS-MAGIC_NUMBER
.set KERNEL_STACK_SIZE, 4096

.section .text.early 
	.globl loader
	.align 4
	.long MAGIC_NUMBER
	.long FLAGS
	.long CHECKSUM

	loader:
		movl %eax,(boottype)
		movl %ebx,(mbinfo)
		movl $(kernel_stack+KERNEL_STACK_SIZE),%esp /*set up kernel stack*/
		call early_gdt
		call early_paging
		jmp spin_up

	loop:
		jmp loop

	.globl finilize_gdt
	finilize_gdt:
		mov $0x10,%ax
		mov %ax,%ds
		mov %ax,%es
		mov %ax,%fs
		mov %ax,%gs

		mov $0x18,%ax
		mov %ax,%ss

		ljmp $0x08,$reload;
		reload:
		nop
		nop
		ret

	.globl early_gdt
	early_gdt: //Here we set up a working-only GDT.
		movw $31,(early_gdt_entry)
		movl $early_gdt_table,(early_gdt_entry+2)
		//Set 4 items for GDT.
		movl $0x0000ffff,(early_gdt_table)
		movl $0x00cf1000,(early_gdt_table+4)

		movl $0x0000ffff,(early_gdt_table+8)
		movl $0x00cf9b00,(early_gdt_table+12)

		movl $0x0000ffff,(early_gdt_table+16)
		movl $0x00cf9300,(early_gdt_table+20)

		movl $0x0000ffff,(early_gdt_table+24)
		movl $0x00cf9300,(early_gdt_table+28)
		//Enable GDT!
		lgdt (early_gdt_entry)
		call finilize_gdt
		ret
	
	.globl early_paging
	early_paging:

		mov $(1024*3),%ecx
		xor %eax,%eax
		xor %edi,%edi
		cld
		rep stosl
		mov $0,%eax
		cld

		/*A PD entry represents 4M,a PTE maps 4K. 10-10-12*/
		movl $(3+0x1000),%eax ;//(pt + present user rw)
		movl %eax,(0x0) ;//Setup PDs.

		//PTEs begin at 0x1000 to 0x3000.
		movl $(0x400000-0x1000+3),%eax ;//End address + (rw kern p)
		movl $(0x2000-4),%edi
		std

	fill_table:
		stosl
		subl $0x1000,%eax
		jge fill_table

		xor %eax,%eax
		mov %eax,%cr3
		mov %cr0,%eax
		or $0x80000000,%eax
		mov %eax,%cr0
		ret
.bss
	.align 4 
	.globl boottype
	.globl mbinfo
	.globl kernel_stack
	//.globl early_pgtable
	//.align 4096
	//.comm early_pgtable,8192


	.comm kernel_stack,KERNEL_STACK_SIZE
	.comm boottype,4
	.comm mbinfo,4

	.globl early_gdt_entry
	.globl early_gdt_table
	.comm early_gdt_entry,8
	.comm early_gdt_table,64


