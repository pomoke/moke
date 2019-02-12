.text
	.globl io_inb
	.globl io_outb
	
	io_outb:
		movb 8(esp),%al
		mov 4(esp),%dx
		out %dx,%al 
		ret
