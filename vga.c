#include "header/type.h"
#include "header/link.h"

int pos_x=0,pos_y=0;

void vga_write(u32 x,u32 y,char c,u8 color)
{
	u8 *fb=(u8 *)0xb8000;
	fb[2*(x+y*80)]=c;
	fb[2*(x+y*80)+1]=color;
	return;
}

void print(char * str,u8 fg,u8 bg)
{
	char *p=str;
	while (*p)
	{
		vga_write(pos_x,pos_y,*p,((bg&0x0f)<<4|(fg&0x0f)));
		pos_x++;
		if (pos_x>=80)
		{
			pos_x=0;
			pos_y++;
		}
		p++;
	}
	pos_x=0;
	pos_y++;
	return;
}
