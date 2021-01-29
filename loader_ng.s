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

	jmp spin_up

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

	.globl early_gdt
	early_gdt: //Here we set up a working-only GDT.
		movw $31,(early_gdt_entry) //We have 4 items,each 8B,and GDT requires -1.
		movl early_gdt_table,2(early_gdt_entry)
		//Set 4 items for GDT.
		movl ,(early_gdt_entry)
		//Enable GDT!
		lgdt (early_gdt_entry)
		ret
	
	.globl early_paging
	early_paging:
		mov $0,%eax
		mov $1024*2,%ecx //We map 0M-2M identically at early boot stage.
		mov $0x0,%edi //Page entries at 0x0
		rep stosl
		pushf
		cld
		
		mov $,(early_pgd)
		mov $,4(early_pgd)

		mov $(0x200000+3),%eax //End address + (rw kern p)
		mov $(early_gpt+8192),%edi
		std
	fill_table:
		stosd
		sub %eax,$0x1000
		jge fill_table;

		mov early_pgd,%cr3
		mov %cr0,%eax
		or $0x80000000,%eax
		mov %eax,%cr0
		ret
.bss
	.align 4
	.globl boottype
	.globl mbinfo
	.globl kernel_stack
	.comm kernel_stack,KERNEL_STACK_SIZE
	.comm boottype,4
	.comm mbinfo,4

	.globl early_gdt_entry
	.globl early_gdt_table
	.globl eayld_pgd
	.comm early_gdt_entry,6
	.comm early_gdt_table,32
	.comm early_pgd,8
	.comm early_pgt,8192
