#define NULL 0
#define MMAP_MAX 32
#define KERNEL_OFFSET 0xc0000000
#define PAGE_SIZE 4096

void *kalloc(u32 size,u32 type);
void kfree(void *addr);
void *palloc(u32 n,u32 type);
void pfree(void *addr);

void * kva_alloc(u32 n);
void kva_init(void);
void map_page(void *v,void *p);

u32 page_get_free(void);
