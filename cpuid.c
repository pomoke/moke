#include <type.h>

int check_cpuid(void)
{
	;
}


char vendor[13];
void __cpuid(u32 op,u32 *b,u32 *d,u32 *c)
{
	asm("cpuid":"=b"(*b),"=d"(*d),"=c"(*c):"a"(op):"memory");
	return;
}

char * cpuid_vendor(void)
{
	//asm("cpuid":"=b"(vendor),"=d"(c),"=c"(d):"a"(0):"memory");
	__cpuid(0,(int *)vendor,(int *)vendor+1,(int *)vendor+2);
	vendor[13]='\0';
	return vendor;
}

int cpuid_feature(int feature)
{
	u32 eax,unused,edx;
	__cpuid(1,&eax,&unused,&edx);
	return edx&feature;
}
