#include <type.h>
#include <io.h>
#include <mem.h>
#include <stdarg.h>
#include <kprint.h>
#include <power.h>
void panic(char * reason,...)
{
	va_list args;
	va_start(args,reason);
	printk("Panic: ");
	vprintk(reason,args);
	printk("\nip %x",__builtin_return_address(1));
	printk("\nSystem halted.");
	halt();
} __attribute__((noreturn));
