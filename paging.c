/* Paging handler. */
#include <type.h>
#include <io.h>
#include <mem.h>
#include <link.h>
#include <kprint.h>
struct int_frame {
	u32 eip;
	u32 cs;
	u32 eflags;
};

struct pde {
	u32 present:1;
	u32 write:1;
	u32 user:1;
	u32 write_through:1;
	u32 nocache:1;
	u32 access:1;
	u32 size:1;
	u32 ignore:1;
	u32 data:3;
	u32 pt:20;
};

struct pte {
	u32 present:1;
	u32 write:1;
	u32 user:1;
	u32 write_through:1;
	u32 nocache:1;
	u32 access:1;
	u32 dirty:1;
	u32 zero:1;
	u32 size:1;
	u32 avail:3;
	u32 phy:20;
};

void isr pf_handler(struct int_frame * frame)
{
	u32 addr;
	asm volatile("mov %%cr2,%%eax":"=a"(addr)::);
	printk("page fault addr %x",addr);
}

struct pde *get_cr3(void)
{
	struct pde *cr3;
	asm volatile("mov %%cr3,%%eax":"=r"(cr3)::"memory");
	return cr3;
}

void load_cr3(void *cr3)
{
	asm volatile("mov %%eax,%%cr3"::"a"(cr3):);
}

u32 get_physical(u32 virtual)
{
	struct pde *dir;
	dir=get_cr3()+0xc0000000;
	printk("paging: cr3 at %x,",dir,sizeof(struct pde));
	//Translate this dir.
	struct pte *item;
	if (!dir[virtual>>22].present)
	{
		printk("paging:No such mapping for %x.\n",virtual);
		return 0;
	}
	else
	{
		item=(dir[virtual>>22].pt)<<11;
		printk("PD table at %x,PT table at %x,",&dir[virtual>>22],item);
		//Translate pte.
		if (!item[(virtual>>12)&0x3ff].present)
		{
			printk("not found\n");
			return 0;
		}
		else
		{
			printk("addr %x -> %x\n",virtual,item[(virtual>>12)&0x3ff].phy<<12);
			return item[(virtual>>12)&0x3ff].phy<<12 | (virtual&0x3ff);
		}			
	}

}
void flush_page(void *virtual)
{
	asm volatile("invlpg (%0)"::"r"(virtual):"memory");
	return;
}

void invalidate_page(void *virtual)
{
	//struct pte *a=get_pg_item(virtual);
	struct pte *a=0;
	a->present=0;
	//asm volatile("invlpg (%0)"::"r"(virtual):"memory");
	flush_page(virtual);
}

void invalidate_all(void)
{
	//Reload %cr3
	asm volatile("mov %%cr3,%%eax;mov %%eax,%%cr3":::"eax");
}

/* This function allocates a page when PT does not exist.Do not use this until mm is ready.
 * However pages within KERNEL_OFFSET+0x0~0x300000 is always safe to map.
 */
void map_page(void *v,void *p) //v->p
{
	//Find PD table
	struct pde *pd=get_cr3();
	struct pte *pt,*phy;
	if (!pd[(u32)v>>22].present)
	{
		//pt=palloc(1)
		phy=palloc(1,0);//Physical address
		map_page(0xc02fc000,phy);
		pt=0xc02fc000;
		//Set PDE
		*(u32 *)(&pd[(u32)v>>22])=0;
		pd[(u32)v>>22].pt=(u32)phy>>11;
		pd[(u32)v>>22].write=1;
		pd[(u32)v>>22].present=1;
	}
	pt=pd[(u32)v>>22].pt<<11;
	pt[((u32)v>>12)&0x3ff].phy=(u32)p>>12;
	pt[((u32)v>>12)&0x3ff].write=1;
	pt[((u32)v>>12)&0x3ff].present=1;
	pt[((u32)v>>12)&0x3ff].user=(u32)p>>0;
	flush_page(v);
	return ;
}

void unshare_page(void *virtual)
{
	//Some null function now still.
	;
}

void paging_init(void)
{
	//Some null function now as we have initialize early paging.
	;
}
