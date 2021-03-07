#include "header/type.h"
#include "header/func.h"
#include <kprint.h>

struct gdt_entry {
	u16 size;
	u32 addr;
} __attribute__((packed));

struct gdt_t {
	u16 limit_l;
	u16 base_l;
	u8 base_m;
	//GDT access byte
	u8 accessed:1;
	u8 rw:1;
	u8 direction:1; //
	u8 execute:1;
	u8 type:1;
	u8 priv:2;
	u8 present:1;
	//End of GDT access byte
	u8 limit_h:4;
	//GDT flags
	u8 zero:2; //No use.
	u8 mode:1; //protected mode or real mode?
	u8 unit:1; //Page- or Byte- wise
	//End GDT flags
	u8 base_h;
} __attribute__((packed));

struct gdt_entry GDT_ENTRY;
struct gdt_t GDT[6];
/* 
 * We only use protected mode,page-wise and entries grow up.No cross-ring calls as well.
 */
void write_gdt(struct gdt_t *this,u32 base,u32 limit,int exist,int ring,int rw,int execute)
{
	if (sizeof(struct gdt_t)!=8)
	{
		kprint("Size of GDT is not 0x08,but ",PR_VGA,INFO);
		kprint_n(sizeof(struct gdt_t),PR_VGA,INFO);
		kprint("\n",PR_VGA,INFO);
	}
	u32 pages	= limit/4096;
	this->limit_l	= pages & 0x0000FFFF;
	this->base_l	= base & 0x0000FFFF;
	this->base_m	= (base & 0x00FF0000)>>(16);
	this->accessed	= 0;
	this->rw	= rw;
	this->direction = 0;
	this->execute	= execute;
	this->type	= 1; //We do not use TSS.
	this->priv	= ring;
	this->present	= exist;
	this->limit_h	= (pages & 0x000F0000)>>(16);
	this->zero	= 0;
	this->mode	= 1;//32-bit
	this->unit	= 1;//Page granuarlity
	this->base_h	= (base & 0xFF000000)>>(24);
	printk("%x %x\n",*((u32 *)this),*((u32 *)this+1));
	return;
}
extern void * kernel_stack;
void setup_gdt(void ** gdt_e,void ** gdt)
{
	write_gdt(GDT,0x0,0xFFFFFFFF,0,0,0,0); //NULL segment
	write_gdt(GDT+1,0x0,0xFFFFFFFF,1,0,1,1);
	write_gdt(GDT+2,0x0,0xFFFFFFFF,1,0,1,0);
	write_gdt(GDT+3,0x0,0xFFFFFFFF,1,0,1,0);
	write_gdt(GDT+4,0x0,0xFFFFFFFF,1,3,1,1);
	write_gdt(GDT+5,0x0,0xFFFFFFFF,1,3,1,0);
	GDT_ENTRY.addr=(u32)GDT;
	GDT_ENTRY.size=4*8-1;
	asm volatile("lgdt (%0)"::"d"(&GDT_ENTRY):);
	*gdt_e=&GDT_ENTRY;
	*gdt=&GDT;
	return;
}

/*void finilize_gdt(void)
{
	asm volatile(
		"jmp 0x08:reload\n"
		"mov %0,%%ss\n"
		"mov %0,%%ds\n"
		"mov %0,%%es\n"
		::"a"(0x10):);
}*/

