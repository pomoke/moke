#define NULL 0
#define MMAP_MAX 32
#define KERNEL_OFFSET 0xc0000000
#define PAGE_SIZE 4096
extern u32 _early_text,_end_early_text;
extern u32 _text,_end_text;
extern u32 _rodata,_end_rodata;
extern u32 _data,_end_data;
extern u32 _bss,_end_bss;
extern u32 _at,_end;

