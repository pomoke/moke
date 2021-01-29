#include <type.h>
#include <io.h>
#include <kprint.h>
#include <list.h>

/* Page Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation

#define PAGE_SIZE 4096 //4k on x86
#define MMAP_MAX 32

#define ALLOC_FREE 1
#define ALLOC_USE 0
struct page_alloc {
	struct page_alloc *prev;
	void *addr;
	u32 len;
	struct page_alloc *next;
};


void * palloc(int n,u32 type) //Alloc n pages.
{
	void * ret=NULL;
	FOR_ITEM(page_list,i)
		if (i->len >= n)
		{
			//Now the entry is breaked into 2 entries.
			ret=i->addr;

		}	
	return ret;
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
