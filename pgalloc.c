#include <type.h>
#include <io.h>
#include <kprint.h>
#include <list.h>
#include <mem_internal.h>

/* Page Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation

#define ALLOC_MEM 3

#define PAGE_SIZE 4096 //4k on x86
#define MMAP_MAX 32

#define ALLOC_FREE 1
#define ALLOC_USE 0
struct alloc {
	struct alloc *prev;
	void *addr;
	u32 len; //Special usage: LSb : page usage: 0 -> unused 1 -> used
	struct alloc *next;
};

struct alloc *page_list,pgl;
void *pgalloc_init(int n,u32 type)
{
	page_list=&pgl;
	struct alloc *p,*this;
	this=page_list;

	page_list->prev=NULL;
	page_list->addr=0;
	page_list->len=1;
	for (int i=0;i<mem_area_count;i++)
	{
		//Create alloc items.
		p=kalloc(sizeof(struct alloc),ALLOC_MEM);
		p->addr=mem_area[i].from;
		p->len=mem_area[i].to-mem_area[i].from;
		p->next=p;
		this->prev=p;
		this->next=NULL;
		this=p;
	}
	return;
}

void * palloc(int n,u32 type) //Alloc n pages.
{
	void * ret=NULL;
	struct alloc *p;
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
	if ((u32)page & 0xfff) //Not a page!
		printk("mem: attempt to pfree() at non-page addr %x\n",page);
	else
	{
		int flag=0;
		//Traverse page link table and find the page.
		FOR_ITEM(page_list,i)
			if (i->addr==page && (u32)(i->addr)&1)
			{
				flag=1;
				i->addr=(void *)((u32)i->addr&0xfffffffe); //Free this memory segment.
				if (PREV(i) && !((u32)(i->prev->addr)&1)) //Left join
				{
					i->addr=i->prev->addr;
					i->len+=i->prev->len;
					i->prev=i->prev->prev;
					kfree(i->prev);
				}
				if (i->next && !((u32)i->next->addr&1)) //Right join
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
