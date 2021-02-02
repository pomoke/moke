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
struct alloc {
	struct page_alloc *prev;
	void *addr;
	u32 len; //Special usage: LSb : page usage: 0 -> unused 1 -> used
	struct page_alloc *next;
};


void * palloc(int n,u32 type) //Alloc n pages.
{
	void * ret=NULL;
	struct page_alloc *p;
	FOR_ITEM(page_list,i)
		if (i->len >= n)
		{
			//Now the entry is breaked into 2 entries.
			ret=i->addr;
			p=kalloc(sizeof(struct alloc),ALLOC_MEM);
			p->prev=i->prev;
			p->addr=i->addr;
			p->len=n;
			p->next=i;
			i->addr=i->addr+n;
			i->len-=n;
			i->prev=p;
		}	
	return ret;
}

void pfree(void *page)
{
	if (page & 0xfff) //Not a page!
		kprint("mem: attempt to pfree() at non-page addr %x\n",page);
	else
	{
		int flag=0;
		//Traverse page link table and find the page.
		FOR_ITEM(page_list,i)
			if (i->addr==page && i->addr&1)
			{
				flag=1;
				i->addr=i->addr&0xfffffffe; //Free this memory segment.
				if (PREV(i) && !(i->prev->addr&1)) //Left join
				{
					i->addr=i->prev->addr;
					i->len+=i->prev->len;
					i->prev=i->prev->prev;
					kfree(i->prev);
				}
				if (i->next && !(i->next->addr&1)) //Right join
				{
					i->len+=i->next->len;
					i->next=i->next->next;
					kfree(i->next);
				}
			}
		if (!flag)
			printk("mem: Attempt to free %x",page);
	}
	return;
}
