/* Virtual Address Space allocator
 *
 * palloc() gets linear address,and dereferrenceing the pointer
 * leads to immediate page fault.
 *
 * So physical address must be mapped to virtual address.
 *
 */
#include <type.h>
#include <list.h>
#include <mem_internal.h>
//#define KERNEL_OFFSET 0xc0000000
//#define PAGE_SIZE 4096

/*
 * Kernel Region: 0xc0000000 - 0xffffffff
 * BASE+0x0-0x300000 is reserved.
 * 
 *
 */
char kva_zone[262143]; //Each byte represents a page ref count in kernel zone,
void kva_ref(void * addr,u32 n)
{
	u32 t=((u32)addr-KERNEL_OFFSET)/PAGE_SIZE;
	for (u32 i=0;i<n;i++)
		kva_zone[t+i]++;
	return;
}
void kva_unref(void * addr,u32 n)
{
	return;
}
void * kva_alloc(u32 n)
{
	//Find continous memory region of n
	void * ret=0;
	int flag=0;
	for (u32 i=0;i<262143;i++)
	{
		flag=1;
		for (u32 j=0;j<n && i+j<262143;j++)
		{
			if (kva_zone[i+j])
			{
				flag=0;
				i+=j;
				break;
			}
		}

		if (flag)
		{
			ret=0xc0000000+4096*i;
			for (u32 j=0;j<n;j++)
				kva_zone[i+j]+=1;
			break;
		}
	}
	return ret;
}

void kva_init(void)
{
	for (int i=0;i<768;i++)
		kva_zone[i]=1;
	for (int i=768;i<262142;i++)
		kva_zone[i]=0;
	return;
}
/*
 * 
 * region: 0x0 - 0xc0000000
 * I: Should this function exist??
 *
 */
void * uva_alloc(u32 n)
{
	;
}
