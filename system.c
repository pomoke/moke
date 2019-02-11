#include "header/link.h"

asmlink int sum(int a,int b,int c)
{
	return a+b+c;
}

asmlink void disp_color(char color)
{
	volatile char *fb=(char *)0x000B8000;
	//clear
	for (int i=0;i<80*25;i++)
	{
		fb[i*2]=' ';
		fb[i*2+1]=color;
	}
	return;
}

asmlink void spin_up(void)
{
	volatile int x;
	x=0;
	disp_color(0x41);
	while (x<100000000)
		x++;
	disp_color(0x20);
	x=0;
	while (x<100000000)
		x++;
	disp_color(0x0);
}

