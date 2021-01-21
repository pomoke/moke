#include <type.h>
#include <io.h>
#include <kprint.h>

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation
void mem_init(int n,struct mem_t *mem)
{
	for (struct mem_t *p=mem;p++;p<mem+n)
		;
}

void kalloc(u32 size,u32 type)
{
	;
}


