/*
 *
 * An implementation of ROMFS.
 *
 */
#include <type.h>
#include <endian.h>
#include <panic.h>
#include <page.h>
#include <string.h>
#include <kprint.h>
#include <vfs.h>
#include <mem.h>

#define ROMFS_MAGIC_1 0x6d6f722d
#define ROMFS_MAGIC_2 0x2d736631

struct romfs {
	int type;
	u32 len;
	union {
		struct superblock *p; //0
		int dev_node; //1
	};
};

//This macro calculate the result of alignment. x: bits
#define ALIGN(x) ( ((x)&0xfffffff0) + 16*(!!((x)&&0xf)) )
#define ADDR(offset) \
	((u32)(offset)+(u32)romfs_addr)

#define HEADER(x) ((x)&0xfffffff8)

#define TYPE(x) ((x)==0 ? "link" : (x)==1 ? "dir" : (x)==2 ? "file" :\
	(x)==3 ? "symlink" : (x==4) ? "block" : (x)==5 ? "char" :\
	(x)==6 ? "socket" : (x)==7 ? "fifo" : "?")

#define FOLLOW_LINK(x) (be_32((x)->info))
//#define NEXT_HEADER(x) (((u32)(x)+be_32((x)->next&0xfffffff0)-1))
#define NEXT_HEADER(x) (((be_32((x)->next))&0xfffffff0)+(u32)(romfs_addr))
#define HEADER_BY_OFFSET(x) (((x)&0xfffffff0)+(u32)(romfs_addr))

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
int romfs_checksum(struct superblock *from,int size);

void romfs_dump_header(struct header *a,int depth)
{
	if (depth>31)
	{
		printk("FS recursion limit reached.Bailing out.\n");
		return;
	}
	struct header *p;
	//printk("header +%d,next +%d,size %d,type %s,name %s\n",(u32)a-(u32)romfs_addr,HEADER(be_32(a->next)),be_32(a->size),TYPE(be_32((u32)a)&0x7),a->name);
	//Should be iterative here.	
	for (p=a;p!=romfs_addr;)
	{
		printk("header +%d,next +%d,size %d,type %s,name %s\n",(u32)p-(u32)romfs_addr,HEADER(be_32(p->next)),be_32(p->size),TYPE(be_32(p->next)&0x7),p->name);
		switch (be_32(p->next) & 0x7) //Check attribute bit.
		{ 
			//TODO: should handle ROMFS_SYMBOL. 
			case ROMFS_LINK:
				if (!strequ(p->name,".") && !(strequ(p->name,"..")))
				{
					p=ADDR(FOLLOW_LINK(p));
					if (p != (struct header *)romfs_addr)
					{
						romfs_dump_header(p,depth+1);
					}
				}
				else
				{
					;
				}
				break;
			case ROMFS_DIR:
				//Check into.
				romfs_dump_header(HEADER_BY_OFFSET(be_32(p->info)),depth+1);
				break;
			default:
				//if (NEXT_HEADER(p) != romfs_addr)	
				//romfs_dump_header(NEXT_HEADER(p),depth+1);
				break;
		}
		p=NEXT_HEADER(p);
		printk("next header at %x\n",p);
	}
}

int romfs_init(char *from,u32 len)
{
	//Check if the image is for romfs.
	BUG_ON(!from)
	printk("ALIGN(20)=%d\n",ALIGN(20));
	if (*(u32 *)from==ROMFS_MAGIC_1 && *(((u32 *)from)+1)==ROMFS_MAGIC_2)
	{
		romfs_addr=(struct superblock *)from;
		romfs_len=len;
		printk("romfs: Found romfs at %x\n",romfs_addr);
		//Print info about this fs.
		printk("romfs: size %d,label %s\n",be_32(romfs_addr->size),romfs_addr->label);
		//Set '/'
		printk("romfs: checksum %s.\n",romfs_checksum(romfs_addr,romfs_addr->size >= 512 ? 512 : romfs_addr->size) ? "Bad" : "OK");
		root_dir=(struct header *)((u32)romfs_addr+16+ALIGN(strlen(romfs_addr->label)));
		printk("root_dir at %x,stlen(label)=%d\n",root_dir,strlen(romfs_addr->label));
		printk("Reading root dir\n");
		//romfs_dump_header(root_dir,0);
		return 1;
	}
	else
	{
		printk("This is not a valid Linux ROMFS.\n");
		return 0;
	}
}
int romfs_checksum(struct superblock *from,int size)
{
	/*
	See linux kernel fs/romfs/super.c .Checksum was not explained detailed in doc.
	First 512 or size bytes,which is shorter,will be used to calculate checksum.
	Add all u32 words up,and check if the sum is 0.

	Only 0 indicates OK.
	*/
	u32 *p = (u32 *)from;
	u32 sum = 0;

	size >>= 2;
	while (size>0)
	{
		sum += be_32(*p); //Pitfull: Avoid using self-inc/decrement in macros.
		p++;
		size--;
	}
	return sum;
}

int romfs_open_mem(struct superblock *from,u32 len,struct romfs *p)
{
	//Check if this is a ROMFS.
	if (!(*(u32 *)from==ROMFS_MAGIC_1 && *(((u32 *)from)+1)==ROMFS_MAGIC_2))
	{
		printk("romfs: This is not a valid Linux ROMFS.\n");
		return 0;
	}
	if (romfs_checksum(from,len<512?len:512))
	{
		printk("romfs: ROMFS checksum BAD.");
		return 0;
	}
	p->type = 0;
	BUG_ON(len!=from->size);
	p->len = from->size;
	p->p = from;
	return 1;
}

struct mount *mount_romfs_mem(char *from,u32 len,struct mount *m)
{
	struct romfs *control = kalloc(sizeof(struct romfs),0);

	if (!romfs_open_mem(from,len,control))
	{
		return 0;
	}

	//Path is filled by caller. FS driver does not need to know about mountpoint.
	m->flag = 0;
	m->backend.type = BACKEND_MEM;
	m->config = control;

	return m;
}