#include <type.h>
#include <io.h>
#include <kprint.h>

/* Memory Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation
#define ALLOC_MEM 3 //Memory allocation routines only.

#define PAGE_SIZE 4096 //4k on x86
#define MMAP_MAX 16  

/* About memory zones for memory allocator
 * 
 * The zone is allocated from lower addr to higher addr.
 * Once the zone is 'full',allocator scans the items and try to get a freed space.
 *
 */
struct mmap
{
	void *from;
	void *to;
};

struct alloc {
	struct alloc *prev;
	void *addr;
	u32 len;
	struct alloc *next;
};

struct pre_zone {
	void *addr;
	u32 len;	//In bytes.
	u32 used; 	//In bytes.
}
static struct mmap mem_area[MMAP_MAX];
static u32 mem_area_count;
void mem_init(void)
{
	//Reserve area below 1M and the kernel (~1M)
	mem_area_del((void *)0x100000,(void *)0x200000);
	//We do not use areas below 1M.
	return;
}

void mem_area_del(void *from,void *to) //Remove a area from mmap regions
{
	;
	return;
}

void mem_area_add(void *from,void *to) //Add a area from mmap regions.
{
	mem_area[mem_area_count].from=from;
	mem_area[mem_area_count].to=to;
	mem_area_count++
	return;
}
/* addr and len should be aligned. */
void pre_assign_area(void *addr,u32 len) //This area is used for allocation requests from memory allocators.
{
	pre_zone.addr=addr;
	pre_zone.len=len;
	return;
}
void * kalloc(u32 size,u32 type)
{
	u32 n;
	n=(size/4)*4+ size%4 ? 0 : 4 ; //Be aligned.
	if (type==ALLOC_MEM)
		return mem_alloc(u32 size);
	void * alloc=NULL;
	int cnt=2;
	
try_alloc:
	FOR_ITEM(alloc,i)
	{
		if (i->size>=n)
		{
			;
		}
	}
	if (!alloc && cnt)
	{
		cnt--;
		//Get some pages from page allocator.
		goto try_alloc;
	}
	return alloc;
}

void kfree(void *ptr)
{
	return;
}

