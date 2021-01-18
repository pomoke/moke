#include "header/type.h"
#include "header/io.h"
#include "header/kprint.h"

/* We do not use 16-bit mode here and everywhere!*/
#define ISR_TASK 0x5
#define ISR_INTR 0xE
#define ISR_TRAP 0XF 


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

__attribute__((interrupt)) void generic_isr(struct interrupt_frame *frame)
{
	kprint("Interrupt!\n",PR_VGA,INFO);
	int_ack();
	return;
}

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
	for (u32 i=0;i<255;i++)
	{
		idt_write(IDT+i,generic_isr,ISR_INTR,0);
	}
	idt_load(&idt_entry,IDT,255);
	kprint_n(&idt_entry,PR_VGA,INFO);
	return;
}
