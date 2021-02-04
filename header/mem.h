#define NULL 0
#define MMAP_MAX 32

void *kalloc(u32 size,u32 type);
void kfree(void *addr);
void palloc(u32 n);
void pfree(void *addr);
