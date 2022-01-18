/*
 *
 *
 * VFS subsystem.
 * 
 * Adapter for filesystems and file API.
 *
 */

#include <type.h>

struct vnode_buf {
	;
};

struct vnode {
	int refcnt;
	struct vnode_buf;
	char path[];
};

struct stat {
	int perm; // Unix style 4-octet permission
	int type; // Indicates file type.
	int dev;
	int size;
};

struct dirent {
	int a;
	char name[];
};

struct dir {
	struct dirent *subs;
};
struct mount;
struct backend {
	enum  {
		BACKEND_MEM,
		BACKEND_DEV,
		BACKEND_FILE,
		BACKEND_BIND,
		BACKEND_MOUNT,
	} type;
	union {
		void *ptr; //In-Memory storage
		int dev; //Device backend
		char *name; // File backed mount or mount
		struct mount *mount; //Mount reference
	} backend;
};

struct mount {
	//Mountpoint in absolute path.
	char *path;
	//Config
	int flag; 
	struct backend backend;
	void *config; //FS-specific config
	//Operations callbacks
	int (*open)(void *config,char *path);
	int (*close)(int handle);
	struct dir (*read_dir)(void *config,char *path);
};

struct fs {
	//Pointers to function to mount.NULL refers to un-implemented funcs.
	struct mount *(*mount_from_mem)(int from,struct mount *m);
	struct mount *(*mount_dev)(int dev,struct mount *m);
	struct mount *(*mount_file)(char *path,struct mount *m);
	struct mount *(*mount_bind)(char *path,struct mount *m);
	struct mount *(*mount_ref)(struct mount *from,struct mount *m);

	//Generic umount. If this function is NULL,de-register mount info,no more operations needed.
	int *(*umount)(struct mount *m);

	struct fs *next;
	char name[];
};


void parse_path(char *path)
{
	;
}

int open(const char *pathname,int mode,struct file *file)
{
	;
}

void close(struct file *file)
{
	;
}