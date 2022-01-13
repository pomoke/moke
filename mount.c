#include <type.h>
#include <vfs.h>
#include <string.h>
#include <kprint.h>
#include <mem.h>
struct fs {
    struct fs *next;
    char name[];
};

struct fs *fs_head;

struct fs *search_fs(char *name)
{
    struct fs *ret=fs_head;
    while (ret)
    {
        if (strcmp(ret,name)==0)
        {
            return ret;
        }
        ret=ret->next;  
    }
    return NULL;
}

int mount_mem(char *path,char *fs,void *mem)
{
    struct fs *fs=search_fs(fs);
    //Search in fs.

}

int mount_dev(char *path,char *fs,int dev)
{
    ;
}

int mount_bind(char *mount_path,char *fs,char *bind_path)
{
    ;
}

int mount_file(char *mount_path,char *fs,char *file_path)
{
    ;
}

void fs_register(struct fs *fs)
{
    ;
}