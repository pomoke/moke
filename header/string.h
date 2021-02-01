
#ifdef ARCH_X86
void strcpy(char * dst,char * src)
{
	asm volatile("repnz movsb;"
			"" :"":"":"memory" );
	return ;
}
#else
#endif
