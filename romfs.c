/*
 * A implementation of ROMFS.
 *
 */
#include <type.h>
#include <endian.h>
#include <panic.h>
#include <page.h>
#include <string.h>
#include <kprint.h>

#define ROMFS_MAGIC_1 0x6d6f722d
#define ROMFS_MAGIC_2 0x2d736631



//This macro calculate the result of alignment. x: bits
#define ALIGN(x) ( ((x)&0xfffffff0) + 16*(!!((x)&&0xf)) )
#define ADDR(offset) \
	((offset)+(u32)romfs_addr)

#define HEADER(x) ((x)&0xfffffff8)

enum {
	ROMFS_LINK=0,
	ROMFS_DIR=1,
	ROMFS_FILE=2,
	ROMFS_SYMBOLIC=3,
	ROMFS_BLOCK=4,
	ROMFS_CHAR=5,
	ROMFS_SOCKET=6,
	ROMFS_FIFO=7,
};

struct superblock {
	u32 magic_1;
	u32 magic_2;
	u32 size;
	u32 checksum; //unchecked indeed.
	char label[]; //16-byte aligned
};

struct header {
	u32 next;
	u32 info;
	u32 size;
	u32 checksum; //leave unchecked
	char name[]; //16-byte aligned
};

struct superblock *romfs_addr;
struct header *root_dir;
u32 romfs_len;

void romfs_dump_header(struct header *a)
{
	printk("header %x+%d=%x,next +%d,size %d,name %s\n",romfs_addr,(u32)a-(u32)romfs_addr,a,HEADER(be_32(a->next)),be_32(a->size),a->name);
}

int romfs_init(char *from,u32 len)
{
	//Check if the image is for romfs.
	BUG_ON(!from)
	printk("ALIGN(20)=%d\n",ALIGN(20));
	if (*(u32 *)from==ROMFS_MAGIC_1 && *(((u32 *)from)+1)==ROMFS_MAGIC_2)
	{
		romfs_addr=from;
		romfs_len=len;
		printk("romfs: Found romfs at %x\n",romfs_addr);
		//Print info about this fs.
		printk("romfs: size %d,label %s\n",be_32(romfs_addr->size),romfs_addr->label);
		//Set '/'
		root_dir=(struct header *)((u32)romfs_addr+16+ALIGN(strlen(romfs_addr->label)));
		printk("root_dir at %x,stlen(label)=%d\n",root_dir,strlen(romfs_addr->label));
		printk("Reading root dir\n");
		romfs_dump_header(root_dir);
		return 1;
	}
	else
	{
		printk("Not a romfs.\n");
		return 0;
	}
}


