#include "header/type.h"
#include "header/io.h"
#include <intr.h>
#include <link.h>
void reboot()
{
	//Reboot by 8042 kbd controller
	u8 good=0x02;
	volatile u32 time=0;
	kprint_vga("Rebooting...\n",15,0);
	while (time<100000000)
		time++;
	while (good & 0x02)
		good=inb(0x64);
	outb(0x64,0xFE);
}

void poweroff_qemu()
{
	volatile u32 time=0;
	kprint_vga("Powering off...\n",15,0);
	while (time<100000000)
		time++;
	outw(0x604,0x2000);
}

void _halt(void)
{
	int_stop();
	asm volatile("hlt");
	return;
} __attribute__((noreturn));

void halt(void)
{
	printk("\n----HALT----\n");
	printk("System halted. It is now safe to power off the computer.\n");
	_halt();
}

void reboot_tflt(void)
{
	//idt_write(IDT+8,0,0xe,0);
	//idt_write(IDT+80,0,0xe,0);
	for (int i=0;i<=255;i++)
		idt_write(IDT+i,0,0xe,0);
	idt_reload();
	asm volatile("int $80;":::);
	return;
} __attribute__((noreturn));
