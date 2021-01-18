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
  u64 base;
  u64 len;
  u32 type;
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
  void * mod_addr;
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
};

char * boot_cmdline(struct multiboot *this)
{
  return this->cmdline;
}
char *type[]={"?","RAM","?","ACPI","POWER","RESERVE"};
void show_mmap(struct multiboot *this)
{
	volatile int n;
  if (!(this->flag & 0x20))
  {
	  kprint("No memmap available!\n",PR_VGA,ERROR);
	  return;
  }
  kprint("memmap entry ",PR_VGA,INFO);
  kprint_n(((u32)this->mmap_addr)&0xFFFFFFFF,PR_VGA,INFO);
  struct mb_mem *p=this->mmap_addr;
  int count=this->mmap_length;
  kprint(" items ",PR_VGA,INFO);
  kprint_n(count,PR_VGA,INFO);
  kprint("\n\n",PR_VGA,INFO);
  kprint_n(count,PR_VGA,INFO);
  kprint("Memmap:\n",PR_VGA,INFO);
  for (int i=0;i<count;i++,p=(struct mb_mem *)((u32)p+p->size+sizeof(p->size)))
  {
	  n=0;
	  while (n<200000000)
		  n++;
      kprint_n(p->base,PR_VGA,INFO);
      kprint(" len ",PR_VGA,INFO);
      kprint_n(p->len,PR_VGA,INFO);
      kprint(" type ",PR_VGA,INFO);
      kprint(type[p->type],PR_VGA,INFO);
      kprint("\n",PR_VGA,INFO);
  }
  return;
}
