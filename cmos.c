#include "header/type.h"
#include "header/io.h"
#include "header/kprint.h"
#include "header/intr.h"

u8 disable_nmi;

void cmos_write(u8 reg,u8 val)
{
	outb(0x70,(disable_nmi<<7)|reg);
	outb(0x71,val);
	return;
}

u8 cmos_read(u8 reg)
{
	outb(0x70,(disable_nmi<<7)|reg);
	return inb(0x71);
}
