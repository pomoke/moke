#include <type.h>
#include <io.h>
#include <kprint.h>

/* Memory Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation

#define PAGE_SIZE 4096 //4k on x86
#define MMAP_MAX 32

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

struct page_alloc {
	struct page_alloc *prev;
	void *addr;
	u32 n;
	struct page_alloc *next;
};

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

void * palloc(int n,u32 type) //Alloc n pages.
{
	;
	return;
}

void * kalloc(u32 size,u32 type)
{
	;
	return;
}

void pfree(void *page)
{
	if (page & 0xfff) //Not a page!
	{
		kfree(page);
		kprint("mem: attempt to pfree() at non-page addr %x\n",page);
	}
	else
	{
		//Traverse page link table.
	}
}

void kfree(void *ptr)
{
	return;
}
