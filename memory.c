#include <type.h>
#include <io.h>
#include <kprint.h>
#include <list.h>
#include <mem_internal.h>
#include <mem.h>
#include <string.h>
#include <panic.h>
/* Memory Allocator */

#define ALLOC_SCHED 0 //May sleep.Do not use in intrs. I:This is the default
#define ALLOC_INT 1 //atmoic allocation
#define ALLOC_EMERG 2 //Emergency allocation
#define ALLOC_MEM 3 //Memory allocation routines only.
//#define NULL 0
//#define PAGE_SIZE 4096 //4k on x86
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

struct alloc_table {
	struct alloc item[64];
	u8 len;
	u8 status[64];
};
struct alloc *alloc_list,pre[100];

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
	for (u32 i=0;i<mem_area_count;i++)
		count+=(mem_area[i].to-mem_area[i].from)/0x1000;
	printk("mem: total avail pages %d\n",count);
	pre_assign_area(4096);
	return;
}
void show_usable_mem(void)
{
	for (u32 i=0;i<mem_area_count;i++)
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
	if (from==0x100000)
		from=0x300000;
	mem_area[mem_area_count].from=from;
	mem_area[mem_area_count].to=to;
	mem_area_count++;
	return;
}
/* addr and len should be aligned. */
void pre_assign_area(u32 len) //This area is used for allocation requests from memory allocators when the kalloc() is not ready.
{
	//void * addr;
	//addr=(void *)((u32)mem_area[mem_area_count-1].to-len);
	pre_zone.addr=pre;
	pre_zone.len=sizeof(pre);
	return;
}

struct alloc_table *alloc_table;
u8 in_memzone;
void * mem_alloc(u32 size)
{
	if(!alloc_table)
	{
		void *p=pre_zone.addr+pre_zone.len-size-pre_zone.used;
		pre_zone.used+=size;
		return p;
	}
	else
	{
		u32 p=0;
		for (int i=0;i<64;i++)
			if (alloc_table->status[i]==0)
			{
				alloc_table->len++;
				alloc_table->status[i]++;
				p=&(alloc_table->item[i]);
				break;
			}
		if (p==0)
		{
			printk("No more zone! len %d in %d\n",alloc_table->len,in_memzone);
		}
		//BUG_ON((p || ((u32)p > (u32)alloc_table+sizeof(*alloc_table)) ))
		BUG_ON((u32)p>(u32)alloc_table+sizeof(*alloc_table))
		if (alloc_table->len==58 && !in_memzone)
		{
			in_memzone=1;
			mem_alloc_zone_add();
			in_memzone=0;
		}
		return (void *)p;
	}
}
/* addr is always 8-byte aligned,so the 0-2 bit is used to mark usage.
 *
 * This is a simple SLOB allocator.
 * Memory-footprint friendly but slow and prone to fragmentation.
 *
 */
void kalloc_init(void)
{
	alloc_list=kalloc(sizeof(struct alloc),ALLOC_MEM);
	alloc_list->prev=0;
	alloc_list->next=0;
	alloc_list->addr=0;
	alloc_list->len=0;
	return ;
}

void kalloc_add_region(void * addr,u32 len)
{
	printk("mm: add region %x len %d\n",addr,len);
	FOR_ITEM(alloc_list,i)
		if (!(i->next))
		{
			struct alloc *p;
			p=kalloc(sizeof(struct alloc),ALLOC_MEM);
			p->next=0;
			p->prev=i;
			p->addr=addr;
			p->len=len;
			i->next=p;
			break;
		}
	return;
}
void mem_alloc_zone_add(void)
{
	alloc_table=kalloc(sizeof(struct alloc_table),0);
	clear(alloc_table,sizeof(struct alloc_table));
	return;
}

void kalloc_setup(void)
{
	in_memzone=0;
	printk("Setting up kalloc...\n");
	kalloc_init();
	void *p=pgalloc(16);
	BUG_ON(!p)
	kalloc_add_region(p,16*PAGE_SIZE);
	mem_alloc_zone_add();
	return;
}


void * kalloc(u32 size,u32 type)
{
	u32 n;
	n=(size/8)*8+ (size%8 ? 8 : 0) ; //Be aligned.
	if (type==ALLOC_MEM)
		//return mem_alloc(size);
		return mem_alloc(size);
	if (size>=4096)
		return pgalloc(size&0xfffff000 + !!(size&0xfff)*4096);
	void * alloc=NULL;
	int cnt=2; //We have two chances to scan through the allocation list.
	//int flag=0;
	
try_alloc:
	FOR_ITEM(alloc_list,i)
	{
		if ((i->len >= n) && !(i->len&1) ) //Find a available one.
		{
			//flag=1;
			struct alloc *p=kalloc(sizeof(struct alloc),ALLOC_MEM);
			alloc=i->addr;
			if (alloc==0xc08ae70)
				printk("!\n");
			p->len=n|1;
			p->next=i;
			p->prev=i->prev;
			i->prev->next=i;
			i->addr+=n;
			i->len-=n;
			i->prev=p;
		}
	}
	if (!alloc && cnt)
	{
		cnt--;
		//Get some pages from page allocator.
		printk("Adding page...\n");
		char *p=pgalloc(16);
		kalloc_add_region(p,16*PAGE_SIZE);
		goto try_alloc;
	}
	/*
	if (flag)
		printk("kalloc(%d->%d)=%x\n",size,n,alloc);
	else
		printk("kalloc(%d->%d)=failed\n",size,n);
	*/
	return alloc;
}

void * pgalloc(u32 n) //n pages mapped in kernel address space.
{
	u32 p,v;
	p=palloc(n,0);
	v=kva_alloc(n);
	for (int i=0;i<n;i++)
		map_page(v+PAGE_SIZE*i,p+PAGE_SIZE*i);
	return v;
}

void kfree(void *ptr)
{
	int flag=0;
	FOR_ITEM(alloc_list,i)
		if (i->addr==ptr && i->len&1)
		{
			flag=1;
			//Mark this as free
			i->len=i->len&0xfffffffe;
			//Left join
			if (i->prev && !(i->prev->len&1))
			{
				i->addr=i->prev->addr;
				i->len+=i->prev->len;
				i->prev=i->prev->prev;
				//kfree(i->prev);
			}
			//Right join
			if (i->next && !(i->next->len&1))
			{
				i->len+=i->next->len;
				i->next=i->next->next;
				//kfree(i->next);
			}
		}
	return;
}

void kalloc_test(void)
{
	char *p[10];
	p[0]=kalloc(17,ALLOC_SCHED);
	p[1]=kalloc(17,ALLOC_SCHED);
	printk("kalloc addr %x %x\n",p[0],p[1]);
	return ;
}

void *malloc(u32 n)
{
	return kalloc(n,0);
}
