#include <type.h>
#include <io.h>
#include <kprint.h>
#include <list.h>
/* Memory Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation
#define ALLOC_MEM 3 //Memory allocation routines only.
#define NULL 0
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

struct alloc *alloc_list;

struct pre_zone {
	void *addr;
	u32 len;	//In bytes.
	u32 used; 	//In bytes.
} pre_zone;
struct mmap mem_area[MMAP_MAX];
u32 mem_area_count=0;
void mem_init(void)
{
	//Count usable pages.
	u32 count=0;
	for (int i=0;i<mem_area_count;i++)
		count+=(mem_area[i].to-mem_area[i].from)/0x1000;
	printk("mem: total avail pages %d\n",count);
	pre_assign_area(4096);
	return;
}
void show_usable_mem(void)
{
	for (int i=0;i<mem_area_count;i++)
		printk("mem: usable %x - %x \n",mem_area[i].from,mem_area[i].to);
}
void mem_area_del(void *from,void *to) //Remove a area from mmap regions
{
	;
	return;
}

void mem_area_add(void *from,void *to) //Add a area from mmap regions.
{
	if (from==0x0)
		return;
	if (from=0x100000)
		from=0x300000;
	mem_area[mem_area_count].from=from;
	mem_area[mem_area_count].to=to;
	mem_area_count++;
	return;
}
/* addr and len should be aligned. */
void pre_assign_area(u32 len) //This area is used for allocation requests from memory allocators.
{
	void * addr;
	addr=(void *)((u32)mem_area[mem_area_count-1].to-len);
	pre_zone.addr=addr;
	pre_zone.len=len;
	return;
}
void * mem_alloc(u32 size)
{
	void *p=pre_zone.addr+pre_zone.len-size-pre_zone.used;
	pre_zone.used+=size;
	return p;
}
void * kalloc(u32 size,u32 type)
{
	u32 n;
	n=(size/4)*4+ size%4 ? 0 : 4 ; //Be aligned.
	if (type==ALLOC_MEM)
		//return mem_alloc(size);
		return mem_alloc(size);
	void * alloc=NULL;
	int cnt=2;
	
try_alloc:
	FOR_ITEM(alloc_list,i)
	{
		if (i->len>=n)
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

