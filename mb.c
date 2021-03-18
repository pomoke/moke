#include "header/type.h"
#include "header/kprint.h"

#define BOOTMEM_AVAIL 1
#define BOOTMEM_ACPI 3
#define BOOTMEM_POWER 4
#define BOOTMEM_RESV 5
struct mb_symbol { //A stub only.We have nothing called debugger for now!
  char a[16];
};
struct mb_mem {
  u32 size;
  u32 base_l;
  u32 base_h;
  u32 len_l;
  u32 len_h;
  u32 type;
} __attribute__((packed));

struct mb_mod {
  void * mod_start;
  void * mod_end;
  char * cmdline;
  u32 reserved;
} __attribute__((packed));

struct multiboot {
  int flag;
  //on flag.0
  void * mem_l;
  void * mem_h;
  int bootdev; //We don't use this to determine boot device currently.
  char * cmdline;
  //On flag.3
  int mods_count;
  struct mb_mod * mod_addr;
  //On flag.4,5
  struct mb_symbol syms;
  //On flag.6
  int mmap_length;
  struct mb_mem * mmap_addr;
  //On flag.7 : harddisks
  int drives_length;
  void * drives_addr;
  //On flag.8
  void * config; //BIOS config table.
  //On flag.9
  char * boot_name;
  //On flag.10
  void * apm; //We do not use APM.
  //No bootloader-based framebuffer-setting currently.
} __attribute__((packed));

extern struct multiboot *mbinfo;
char * boot_cmdline(struct multiboot *this)
{
  return this->cmdline;
}
char *type[]={"?","RAM","?","ACPI","POWER","RESERVE"};
void init_mmap(struct multiboot *this)
{
	volatile int n;
  if (!(this->flag & 0x40))
  {
	  kprint("No memmap available!\n",PR_VGA,ERROR);
	  return;
  }
  //kprint("memmap entry ",PR_VGA,INFO);
  //kprint_n(((u32)this->mmap_addr)&0xFFFFFFFF,PR_VGA,INFO);
  struct mb_mem *p=(struct mb_mem *)((void *)this->mmap_addr);
  printk("mem: entry %x length %d\n",this->mmap_addr,this->mmap_length);
  for (;(void *)p<(void *)this->mmap_addr+(u32)this->mmap_length;p=(struct mb_mem *)((u32)p + p->size + sizeof(p->size)))
  {
	  n=0;
	  while (n<200000000)
		  n++;
	printk("mem: %x - %x type %d next %d\n",p->base_l,p->base_l+p->len_l,p->type,p->size);
	if (p->type==1)
	{
		mem_area_add(p->base_l,p->len_l+p->base_l);
	}
	//printk("%d\n",*(int *)0);
	//mem_init();
  }
  mem_init();
  return;
}

void modules_init(void)
{
	if (!(mbinfo->flag & (1<<3)))
	{
		printk("mb:This bootloader does not support modules.\n");
		goto end;
	}
	struct mb_mod *a;
	if (!mbinfo->mods_count)
	{
		printk("mb:No module loaded.\n");
		goto end;
	}
	for (int i=0;i<mbinfo->mods_count;i++)
	{
		a=mbinfo->mod_addr+i;
		printk("module at %x len %d cmdline %s\n",a->mod_start,a->mod_end-a->mod_start,a->cmdline);
		romfs_init(a->mod_start,a->mod_end-a->mod_start);
	}
end:
	return;
}