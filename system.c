#include "header/link.h"

asmlink int sum(int a,int b,int c)
{
	return a+b+c;
}

asmlink void spin_up(void)
{
	char *fb=(char *)0x000B8000;
	fb[0]='A';
	fb[1]=0x41;
	fb[2]='B';
	fb[3]=0x41;
	fb[4]='C';
	fb[5]=0x41;
	fb[40]='F';
	fb[41]=0x41;
	return;
}
