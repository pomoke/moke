#include <link.h>
#include <type.h>
#include <gdt.h>
#include <kprint.h>
#include <mb.h>
#include <intr.h>
#include <clock.h>
#include <power.h>
#include <cpuid.h>
#include <serial.h>
#include <mem.h>
extern int boottype;
asmlink i32 sum(i32 a,i32 b,i32 c)
{
	return a+b+c;
}

/*
asmlink void disp_color(u8 color)
{
	volatile u8 *fb=(u8 *)0x000B8000;
	//clear
	for (i32 i=0;i<80*25;i++)
	{
		fb[i*2]=' ';
		fb[i*2+1]=color;
	}
	return;
}
*/
/*
#define PR_SERIAL 1
#define PR_VGA 2
#define INFO 1
#define KERN 2
#define WARN 3
#define ERROR 4
*/
void kprint(char *str,i16 target,i16 level)
{
	//if (target==PR_SERIAL)
	if (target==PR_VGA)
	{
		switch (level)
		{
			case 1: kprint_vga(str,7,0);break;
			case 2: kprint_vga(str,15,0);break;
			case 3: kprint_vga(str,15,1);break;
			case 4: kprint_vga(str,15,4);break;
		}
	}
	return;
}
char conv[]="0123456789abcdef";
char * nullptr=0;
char * vnullptr=0xc0000000;
void kprint_n(u32 n,i16 target,i16 level)
{
	char s[9];
	s[8]='\0';
	int tmp=n;
	for (int i=7;i>=0;i--)
	{
		s[i]=conv[tmp%16];
		tmp/=16;
	}
	kprint(s,target,level);
	return;
}
extern struct multiboot * mbinfo;
asmlink void spin_up(void)
{
	volatile u32 x=0,y=0;
	u8 *fb=(u8 *)0xb8000;
	for (int i=0;i<25*80;i++)
		fb[2*i]=' ';
	//kprint("Hello\n",PR_VGA,INFO);
	//kprint("MOKE v0.0.1 by pomoke\n",PR_VGA,KERN);
	//kprint("Number tests:",PR_VGA,INFO);
	//kprint_n(13,PR_VGA,INFO);
	//kprint(" ",PR_VGA,INFO);
	//kprint_n(64,PR_VGA,INFO);
	//kprint("\n",PR_VGA,INFO);
	if (boottype!=0x2badb002) 
	{
		printk("Error:not booted by multiboot1 loader!\nSystem halted.");
		for (;;) ;
	}
	printk("Conforming to multiboot standard\n");
	printk("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n");
	//print("H\n",PR_VGA,WARN);
	void *gdt_e,*gdt;
	//setup_gdt(&gdt_e,&gdt);
	//kprint("GDT at ",PR_VGA,INFO);
	//kprint_n((u32)gdt_e,PR_VGA,INFO);
	//kprint(" ",PR_VGA,INFO);
	//kprint_n((u32)gdt,PR_VGA,INFO);
	//kprint(" is loaded\n",PR_VGA,INFO);
	printk("GDT at %x\n",(u32)gdt_e);
	printk("CPU vendor: %s\n",cpuid_vendor());
	x=0;
	while (x<400000000)
	{
		x++;
	}
	printk("Loaded GDT.\n");
	x=0;
	while (x<400000000)
	{
		x++;
	}
	printk("cmdline: %s",boot_cmdline(mbinfo));
	intr_init();
	printk("ISR ready.\n");
	x=0;
	asm volatile("int $0x80":::);
	//asm volatile("mov $0x23,%%ax;mov %%ax,%%ds":::"ax");
	printk("%x\n",get_physical(0xc0200004));
	printk("%x\n",get_physical(0x0));
	init_mmap(mbinfo);
	pgalloc_init();
	//Unset 0x0
	invalidate_page(0);
	kva_init();
	volatile char *q;
	//for (int i=0;i<10;i++)
	//	printk("this %x\n",kva_alloc(100000));
	
	show_pg_list();
	show_usable_mem();
	volatile u32 *p=0xc02fe000;
	*p=3;
	printk("p=%x\n",*p);
	printk("Mapping...\n");
	map_page(0xc02fe000,palloc(1,0));
	map_page(0xd0000000,palloc(1,0));
	p=0xd0000000;
	*p=3;
	//p=0xe0000000;
	//*p=4;
	//printk("p=%x\n",*p);
	x=0;
	//for (int i=0;i<20;i++)
	//	printk("alloc:%x\n",palloc(5000));
	kprint("ISR loaded.\n",PR_VGA,INFO);
	kprint("Starting interrupts...\n",PR_VGA,INFO);
	x=0;
	while (x<400000000)
		x++;
	x=0;

	int_start();
	//Test palloc() and pfree() 
	struct date date;
	rtc_read(&date);
	//kprint(week[date.weekday-1],PR_VGA,INFO);
	int c=0;
	//panic("Nothing to do.");
	uart_set_baud(0x3f8,38400);
	serial_write(0x3f8,'\n');
	serial_write(0x3f8,'a');
	serial_write(0x3f8,'b');
	//printk("%p %p %p %!\n",0xc,0xff,0xdeadbeef);
	printk("allocing pages from %x...\n",palloc(10,0));
	kalloc_setup();
	kalloc_test();
	char *p1=pgalloc(32);
	printk("page free: %d MB\n",page_get_free()/1024/1024);
	//panic("Nothing to do.\n");
	//halt();
	x=0;
	//printk("%d\n",x/x);	
	//clock_now();
	printk("0xc0100000->%x\n",get_physical(0xc0100000));
	show_kernel_zone();
	for (int i=0;i<1000000;i++)
	{
		//p1=kalloc(64,0);
		//p1=pgalloc(1);
	}
	timer_init();
	timer_test();
	for (int i=0;i<200000000;i++)
	{
		tick_handler();
		if (!(i%10000000))
			//printk("test %d\n",i);
			;

	}
	halt();

}

