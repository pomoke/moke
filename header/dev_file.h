
struct dev_block {
    int (*read)(char *buf,int offset,int n);
    int (*write)(char *buf,int offset,int n);
    int (*get_size)(void);
    int (*resize)(int n);
    int (*control)(void *p);
    void (*close)(void);
};

struct dev_stream {
    int (*read)(char *buf,int n);
    int (*write)(char *buf,int n);
    int (*control)(void *p);
    void (*close)(void);
};