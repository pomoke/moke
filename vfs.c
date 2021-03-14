/*
 *
 *
 * VFS subsystem.
 *
 *
 *
 */

#include <type.h>

struct vnode_buf {
	;
}

struct vnode {
	int refcnt;
	struct vnode_buf;
	char path[];
};

struct stat {
	;
}

void parse_path(char *path)
{
	;
}

int *open(const char * pathname,int mode,struct *file file)
{
	;
}

void close(struct *file file)
{
	;
}
