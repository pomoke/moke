#define NULL 0
#define MMAP_MAX 32

struct mmap 
{
	void *from;
	void *to;
};

extern struct mmap mem_area[MMAP_MAX];
extern u32 mem_area_count;
