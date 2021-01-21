#include <io.h>
#include <type.h>
#include <mem.h>
#include <stdarg.h>
#include <kprint.h>
#include <power.h>
void panic(char * reason,...)
{
	va_list args;
	va_init(args,reason);
	printk("Panic: ");
	vprintk(reason,args);
	printk("\nSystem halted.");
	halt();
}
