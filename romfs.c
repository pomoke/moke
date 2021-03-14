/*
 * A implementation of ROMFS.
 *
 */
#include <type.h>

enum {
	ROMFS_LINK=0;
	ROMFS_DIR=1;
	ROMFS_FILE=2;
	ROMFS_SYMBOLIC=3;
	ROMFS_BLOCK=4;
	ROMFS_CHAR=5;
	ROMFS_SOCKET=6;
	ROMFS_FIFO=7;
}

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
