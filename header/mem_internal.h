#define NULL 0
#define MMAP_MAX 32
#define KERNEL_OFFSET 0xc0000000
#define PAGE_SIZE 4096
struct mmap 
{
	void *from;
	void *to;
};

extern struct mmap mem_area[MMAP_MAX];
extern u32 mem_area_count;
