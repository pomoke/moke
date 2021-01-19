#include "header/type.h"
#include "header/io.h"
#include "header/kprint.h"

/* We do not use 16-bit mode here and everywhere!*/
#define ISR_TASK 0x5
#define ISR_INTR 0xE
#define ISR_TRAP 0XF 

/* PIC helpers */
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMM PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMM PIC2
#define PIC2_DATA (PIC2+1)

#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b

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

u8 this_int(void) //Only in ISRs.
{
	outb(PIC1_COMM,PIC_READ_ISR);
	return inb(PIC1_COMM);
}

struct interrupt_frame;

__attribute__((interrupt)) void unknown_isr(struct interrupt_frame *frame)
{
	kprint("Interrupt ",PR_VGA,INFO);
	kprint_n(this_int(),PR_VGA,INFO);
	kprint("\n",PR_VGA,INFO);
	int_ack();
	return;
}

__attribute__((interrupt)) void unknown_fault(struct interrupt_frame *frame,u32 code)
{
	kprint("Unknown Fault.\n",PR_VGA,ERROR);
	return;
}

__attribute__((interrupt)) void div_by_zero(struct interrupt_frame *frame)
{
	kprint("Divide by zero\n",PR_VGA,ERROR);
	return ;
}

__attribute__((interrupt)) void overflow(struct interrupt_frame *frame)
{
	kprint("Overflow.\n",PR_VGA,ERROR);
	return ;
}
__attribute__((interrupt)) void bound_check(struct interrupt_frame *frame)
{
	kprint("Bound check failed.\n",PR_VGA,ERROR);
	return ;
}

__attribute__((interrupt)) void illegal_op(struct interrupt_frame *frame)
{
	kprint("Illegal instruction.\n",PR_VGA,ERROR);
	return ;
}

__attribute__((interrupt)) void nodev(struct interrupt_frame *frame)
{
	kprint("No such device.\n",PR_VGA,ERROR);
	return ;
}

__attribute__((interrupt)) void double_fault(struct interrupt_frame *frame,u32 errno)
{
	kprint("Double fault occurred!\n",PR_VGA,ERROR);
	return ;
}
__attribute__((interrupt)) void  invalid_seg(struct interrupt_frame *frame,u32 errno)
{
	kprint("Attempt to load an invalid segment.\n",PR_VGA,ERROR);
	return ;
}
__attribute__((interrupt)) void gpe(struct interrupt_frame *frame,u32 errno)
{
	kprint("General Protection Fault.\n",PR_VGA,ERROR);
	return ;
}

void (*faults[])(struct interrupt_frame,u32)=
{
	div_by_zero,
	unknown_isr, //trap,not now.
	unknown_isr, //Unused
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
	unknown_fault, //Page fault.No paging for now.
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
	kprint_n(&idt_entry,PR_VGA,INFO);
	return;
}
