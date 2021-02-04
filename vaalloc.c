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

void * kva_alloc(u32 n)
{
	;
}

void * uva_alloc(u32 n)
{
	;
}
