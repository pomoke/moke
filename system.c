#include "header/link.h"
#include "header/type.h"
#include "header/gdt.h"
#include "header/kprint.h"
#include "header/mb.h"
#include "power.h"

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

void kprint_n(u32 n,i16 target,i16 level)
{
	char s[9];
	s[8]='\0';
	int tmp=n;
	for (int i=7;i>=0;i--)
	{
		s[i]="0123456789abcdef"[tmp%16];
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
	kprint("Hello\n",PR_VGA,INFO);
	kprint("MOKE v0.0.1 by pomoke\n",PR_VGA,KERN);
	kprint("Number tests:",PR_VGA,INFO);
	kprint_n(13,PR_VGA,INFO);
	kprint(" ",PR_VGA,INFO);
	kprint_n(64,PR_VGA,INFO);
	kprint("\n",PR_VGA,INFO);
	if (boottype!=0x2badb002) 
	{
		kprint("Error:not booted by multiboot loader!\nSystem halted.",PR_VGA,ERROR);
		reboot();
		for (;;) ;
	}
	kprint("Conforming to multiboot standard\n",PR_VGA,KERN);
	kprint("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n",PR_VGA,WARN);
	//print("H\n",PR_VGA,WARN);
	void *gdt_e,*gdt;
	setup_gdt(&gdt_e,&gdt);
	kprint("GDT at ",PR_VGA,INFO);
	kprint_n((u32)gdt_e,PR_VGA,INFO);
	kprint(" ",PR_VGA,INFO);
	kprint_n((u32)gdt,PR_VGA,INFO);
	kprint(" is loaded\n",PR_VGA,INFO);
	x=0;
	while (x<400000000)
	{
		x++;
	}
	finilize_gdt();
	kprint("Loaded GDT.\n",PR_VGA,INFO);
	x=0;
	while (x<400000000)
	{
		x++;
	}
	show_mmap(mbinfo);
	*(int*)0x10000000=3;
	kprint_n(*(int *)0x10000000,PR_VGA,INFO);
	//poweroff_qemu();
	//asm("hlt");
	x=0;
	for (;;) ;
}

