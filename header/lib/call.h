int syscall(int a,int b,int c,int d)
{
	int ret;
	asm volatile("int $0x80"
			:"=a"(ret)
			:"b"(a),"c"(b),"d"(c),"D"(d)
			:"memory");
	return;
}
