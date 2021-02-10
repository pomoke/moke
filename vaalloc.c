/* Virtual Address Space allocator
 *
 * palloc() gets linear address,and dereferrence to it
 * leads to immediate page fault.
 *
 * So physical address must be mapped to virtual address.
 *
 */
#include <type.h>
#include <list.h>

#define KERNEL_OFFSET 0xc0000000


//0xc0000000 - 0xffffffff
void * kva_alloc(u32 n)
{
	;
}

//0x0 - 0xc0000000
void * uva_alloc(u32 n)
{
	;
}
