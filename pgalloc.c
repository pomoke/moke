#include <type.h>
#include <io.h>
#include <kprint.h>
#include <list.h>
#include <mem_internal.h>
#include <mem_type.h>

/* Page Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation

#define ALLOC_MEM 3

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

void show_pg_list(void)
{
	printk("mem: showing page allocations.\n");
	FOR_ITEM(page_list,i)
		printk("page region %x len %d next %x\n",i->addr,i->len,i->next);
	return;
}
void *pgalloc_init(int n,u32 type)
{
	page_list=&pgl;
	struct alloc *p,*this;
	this=page_list;

	page_list->prev=NULL;
	page_list->addr=0;
	page_list->len=0;
	for (int i=0;i<mem_area_count;i++)
	{
		//Create alloc items.
		p=kalloc(sizeof(struct alloc),ALLOC_MEM);
		printk("mem:storing page item at %x\n",p);
		p->addr=mem_area[i].from;
		p->len=mem_area[i].to-mem_area[i].from;
		p->prev=this;
		p->next=i;
		this->prev=NULL;
		this->next=p;
		this=p;
	}
	return;
}

void * palloc(int n,u32 type) //Alloc n pages.
{
	void * ret=NULL;
	struct alloc *p;
	FOR_ITEM(page_list,i)
	{
		//printk("page entry: %x len %x at %x %s\n",i->addr,i->len&0xfffffffe,i,i->len&1?"used":"unused");
		if (i->len >= n*4096 && !(i->len&1))
		{
			//Now the entry is breaked into 2 entries.
			ret=i->addr;
			p=kalloc(sizeof(struct alloc),ALLOC_MEM);

			p->prev=i->prev;
			p->prev->next=p;
			p->addr=i->addr;
			p->len=n*4096|1;
			p->next=i;
			i->addr=i->addr+n*4096;
			i->len-=n*4096;
			i->prev=p;
		}	
	}
	printk("palloc(%d)=%x\n",n,ret);
	return ret;
}

void pfree(void *page)
{
	if ((u32)page & 0xfff) //Not a page!
		printk("pgalloc: attempt to pfree() at non-page addr %x\n",page);
	else
	{
		printk("pfree(%x)=void \n",page);
		int flag=0;
		//Traverse page link table and find the page.
		FOR_ITEM(page_list,i)
		{
			//printk("page entry: %x len %x at %x %s\n",i->addr,i->len&0xfffffffe,i,i->len&1?"used":"unused");
			if (i->addr==page && i->len&1)
			{
				//printk("pgalloc: Found entry.\n");
				flag=1;
				//i->addr=(void *)((u32)i->addr&0xfffffffe); //Free this memory segment.
				i->len=i->len&0xfffffffe;
				//printk("Left join\n");
				if (PREV(i) && !((u32)(i->prev->len)&1)) //Left join
				{
					i->addr=i->prev->addr;
					i->len+=i->prev->len;
					i->prev=i->prev->prev;
					kfree(i->prev);
				}
				printk("Right join\n");
				if (i->next && !((u32)i->next->len&1)) //Right join
				{
					i->len+=i->next->len;
					i->next=i->next->next;
					kfree(i->next);
				}
				break;
			}
		}
		if (!flag)
			printk("pgalloc: Attempt to free %x",page);
	}
	return;
}

void pgalloc_test(void)
{
	void *p,*q;
	p=palloc(1,0);
	pfree(p);
	q=palloc(1,0);
	printk("palloc_test: %x %x\n",p,q);
	return ;
}
