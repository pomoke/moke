#include "header/type.h"
#include "header/io.h"
#include "header/kprint.h"
#include "header/power.h"
#include <link.h>
/* We do not use 16-bit mode here and everywhere!*/
#define ISR_TASK 0x5
#define ISR_INTR 0xE
#define ISR_TRAP 0XF 

/* PIC helpers */
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2+1)

#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0X04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

/* ICW2 : vector offset
 * ICW3 : M/S wiring
 * ICW4 : Additional info
 */

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0c
#define ICW4_SFNM 0x10

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

#define isr __attribute__((interrupt))
/* The IDT table requires the minimum limit of 100h.
 * There are 256 intr's available.
 */
struct idt_entry {
	u16 limit; //bytes-1
	u32 addr;//Linear address
} __attribute__((packed));

struct idt_entry idt_entry;
/* IA-32 table */
struct idt_desc {
	u16 offset_l;
	u16 cs;
	u8 zero;
	//Control flags
	u8 type:4 ;
	u8 ss:1 ;
	u8 ring:2 ;
	u8 present:1 ;
	u16 offset_h;
} __attribute__((packed));

void int_start(void) //Only use this when we are ready!
{
	asm volatile("sti");
	return;
}

void int_stop(void) //Use this when we are done!
{
	asm volatile("cli");
	return ;
}

void int_ack(void) //PIC version,use this at the end of ISR.
{
	outb(0x20,0x20);
}
u8 pic_get_irq(int ocw3)
{
	outb(PIC1_CMD,ocw3);
	outb(PIC2_CMD,ocw3);
	return (inb(PIC2_CMD)<<8)|inb(PIC1_CMD);
}
u8 this_int(void) //Only in ISRs.
{
	return pic_get_irq(PIC_READ_ISR);
}

struct interrupt_frame {
	u32 eip;
	u32 cs;
	u32 eflags;
};

isr void unknown_isr(struct interrupt_frame *frame)
{
	if (this_int()!=1)
		printk("intr %x\n",this_int());
	int_ack();
	return;
}

isr void unknown_fault(struct interrupt_frame *frame,u32 code)
{
	kprint("Unknown Fault.\n",PR_VGA,ERROR);
	return;
}

isr void div_by_zero(struct interrupt_frame *frame)
{
	printk("fault: Divide by zero at ip %x.\n",frame->eip);
	printk("System halted.");
	halt();
	return ;
}

isr void overflow(struct interrupt_frame *frame)
{
	kprint("fault: Overflow. at ip %x\n",PR_VGA,ERROR);
	return ;
}
isr void bound_check(struct interrupt_frame *frame)
{
	kprint("Bound check failed.\n",PR_VGA,ERROR);
	return ;
}

isr void illegal_op(struct interrupt_frame *frame)
{
	printk("fault: Illegal instruction at ip %x.\nSystem halted.\n",frame->eip);
	halt();
	return ;
}

isr void nodev(struct interrupt_frame *frame)
{
	kprint("No such device.\n",PR_VGA,ERROR);
	return ;
}

isr void double_fault(struct interrupt_frame *frame,u32 errno)
{
	kprint("Double fault occurred!\n",PR_VGA,ERROR);
	return ;
}
isr void invalid_seg(struct interrupt_frame *frame,u32 errno)
{
	kprint("Attempt to load an invalid segment.\n",PR_VGA,ERROR);
	return ;
}
isr void gpe(struct interrupt_frame *frame,u32 errno)
{
	printk("fault:General Protection Fault at %x errno %x\n",frame->eip,errno);
	for (;;);
	return ;
}

isr void page_fault(struct interrupt_frame *frame,u32 errno)
{
	u32 addr;
	asm volatile("mov %%cr2,%0":"=a"(addr)::);
	if (addr)
	{
		printk("fault:Page fault at %x access %x.\n",frame->eip,addr);
	}
	else
		printk("fault:Attempt to deref NULL pointer at %x.\n",frame->eip);
	halt();
	return ;
}

isr void nmi_handler(struct interrupt_frame *frame)
{
	printk("fault:nmi at %x\n",frame->eip);
	halt();
}

void (*faults[])(struct interrupt_frame,u32)=
{
	div_by_zero,
	unknown_isr, //trap,not now.
	nmi_handler, //Unused
	unknown_isr, //int 3
	overflow,
	bound_check,
	illegal_op,
	nodev,
	double_fault,//With errno
	unknown_isr, //x87_seg_overflow
	unknown_fault, //TSS,errno
	invalid_seg,
	invalid_seg,
	gpe, 
	page_fault, //Page fault.This will be a very big handler.
	unknown_isr //x87 error.
};


void idt_write(struct idt_desc *this,u32 func,u8 type,u8 ring)
{
	this->offset_l	= func&0xFFFF;
	this->cs	= 0x08;
	this->zero	= 0;
	this->type	= type;
	this->ss	= 0;
	this->ring	= ring;
	this->present	= 1;
	this->offset_h  = (func&0xFFFF0000)>>16;
	return;
}

struct idt_desc IDT[256];

void idt_load(struct idt_entry *entry,struct idt_desc *idt,u32 limit)
{
	entry->limit=limit*16-1;
	entry->addr=idt;
	asm volatile("lidt (%0)"::"d"(entry):);
	return;
}

void intr_init(void)
{
	for (u32 i=0;i<=0x10;i++)
		idt_write(IDT+i,faults[i],ISR_INTR,0);
	for (u32 i=0x11;i<255;i++)
	{
		idt_write(IDT+i,unknown_isr,ISR_INTR,0);
	}
	idt_load(&idt_entry,IDT,255);
	pic_init();	
	printk("IDT at %x\n",&idt_entry);
	return;
}

void idt_reload(void)
{
	idt_load(&idt_entry,IDT,255);
	return;
}

void set_intr(int n,void (*func)(struct interrupt_frame,u32))
{
	idt_write(IDT+n,func,ISR_INTR,0);
	return ;
}

void pic_remap(int off1,int off2)
{
	u8 a1,a2;
	a1=inb(PIC1_DATA);
	a2=inb(PIC2_DATA);
	outb(PIC1_CMD,ICW1_INIT|ICW1_ICW4);
	outb(PIC2_CMD,ICW1_INIT|ICW1_ICW4);
	outb(PIC1_DATA,off1);
	outb(PIC2_DATA,off2);
	outb(PIC1_DATA,4);
	outb(PIC2_DATA,2);

	outb(PIC1_DATA,ICW4_8086);
	outb(PIC2_DATA,ICW4_8086);

	outb(PIC1_DATA,a1);
	outb(PIC2_DATA,a2);
	return;
}

void pic_init(void)
{
	pic_remap(PIC1_OFFSET,PIC2_OFFSET);
	return;
}

void pic_disable(void)
{
	outb(PIC1_CMD,0xff);
	outb(PIC2_CMD,0xff);
	return ;
}
