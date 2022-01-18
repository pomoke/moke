
struct console {
    //For console handler use. 
    union {
        int data;
        void *ptr;
    } data;
    
    u32 console_cap;
    void (*console_start)(struct console *c);
    int (*console_write)(struct console *c,char *p,u32 len);
    int (*console_write_str)(struct console *c,char *p);

    int (*console_read)(struct console *c,char *buf,u32 len); //Returns length of read content.
    struct console *next;
};

extern struct console *console_in,*console_out;

void console_set_out(struct console *con);
void console_set_in(struct console *con);
void console_set(struct console *con);