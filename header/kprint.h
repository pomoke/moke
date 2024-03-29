#include "type.h"
#define PR_SERIAL 1
#define PR_VGA 2
#define INFO 1
#define KERN 2
#define WARN 3
#define ERROR 4
static int kprint_target;
void kprint_vga(char *str,u8 fg,u8 bg);
void kprintn_vga(char *str,u32 len,u8 fg,u8 bg);
void kprint(char *str,i16 target,i16 level);
void kprint_n(u32 a,i16 target,i16 level);
void setup_gdt(void **gdt_e,void **gdt);
void printk(char *fmt,...);
void set_kprint_target(int a);


