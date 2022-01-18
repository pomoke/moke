#include <type.h>
#include <io.h>
#include <console.h>
#include <serial.h>
#include <uart.h>

struct console serialcon;
/* The serial port meta-driver */
void serial_console_start(struct console *c)
{
    //Interrupt register here.
    return;
}

int serial_console_write(struct console *c,char *p,u32 len)
{
    while (len)
    {
        serial_write(c->data.data,*p++);
        len--;
    }
    return len;
}

int serial_console_write_str(struct console *c,char *p)
{
    while (*p)
    {
        serial_write(c->data.data,*p++);
    }
    return 1;
}

int serial_console_read(struct console *c,char *p,u32 len)
{
    //To be implented.
    return len;
}

void serial_default_console_init(int port)
{
    serialcon.data.data = port;
    serialcon.console_cap = 0xff;
    serialcon.console_write = serial_console_write;
    serialcon.console_write_str = serial_console_write_str;
    serialcon.console_read = serial_console_read;
}