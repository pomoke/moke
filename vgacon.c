#include <type.h>
#include <kprint.h>
#include <console.h>
struct console vgacon;
void vgacon_start(struct console *c)
{
    return 1;
}

int vgacon_write(struct console *c,char *p,int len)
{
    kprintn_vga(p,len,15,0);
}

int vgacon_write_str(struct console *c,char *str)
{
    kprint_vga(str,15,0);
}

//No vgacon_read here,use kbd_read instead.
int vgacon_default_con_init(void)
{
    vgacon.data.data = 0;
    vgacon.console_cap = 0xff;
    vgacon.console_write = vgacon_write;
    vgacon.console_write_str = vgacon_write_str;
    vgacon.console_read = NULL;
    console_out = &vgacon;
}