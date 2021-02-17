
#ifdef ARCH_X86
void strcpy(char * dst,char * src)
{
	asm volatile("repnz movsb;"
			"" :"":"":"memory" );
	return ;
}
#else
#endif

void clear(void * dst,u32 n)
{
	char *p=dst;
	for (u32 i=0;i<n;i++)
		p[i]=0;
	return;
}
