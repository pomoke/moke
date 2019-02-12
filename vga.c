#include "header/type.h"
#include "header/link.h"
#include "header/io.h"

#define VGA_MMIO_ADDR 0xb8000
#define VGA_WIDTH 80
#define VGA_CMD_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5
#define VGA_IO_HIGH 0xe
#define VGA_IO_LOW 0xf
int pos_x=0,pos_y=0;

void vga_cursor(u32 x,u32 y)
{
	outb(VGA_CMD_PORT,VGA_IO_HIGH);
	outb(VGA_DATA_PORT,(((y*VGA_WIDTH+x)>>8)&0xff));
	outb(VGA_CMD_PORT,VGA_IO_LOW);
	outb(VGA_DATA_PORT,(y*VGA_WIDTH+x)&0xff);
}

void vga_write(u32 x,u32 y,char c,u8 color)
{
	u8 *fb=(u8 *)VGA_MMIO_ADDR;
	fb[2*(x+y*VGA_WIDTH)]=c;
	fb[2*(x+y*VGA_WIDTH)+1]=color;
	return;
}

void print(char * str,u8 fg,u8 bg)
{
	char *p=str;
	while (*p)
	{
		if (*p=='\n')
		{
			pos_y++;
			pos_x=0;
		}
		else 
		{
			vga_write(pos_x,pos_y,*p,((bg&0x0f)<<4|(fg&0x0f)));
			pos_x++;
		}
		if (pos_x>=VGA_WIDTH)
		{
			pos_x=0;
			pos_y++;
		}
		p++;
	}
	vga_cursor(pos_x-1,pos_y);
	return;
}
