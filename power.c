#include "header/type.h"
#include "header/io.h"
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

void halt(void)
{
	int_stop();
	//asm volatile("hlt");
	for (;;) ;
	return;
}
