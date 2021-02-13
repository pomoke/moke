#include <type.h>
#include <io.h>
#include <mem.h>
#include <stdarg.h>
#include <kprint.h>
#include <power.h>
u32 status;
void panic(char * reason,...)
{
	if (status==1)
	{
		printk("double panic!\n");
		status++;
		halt();
	}
	else if (status>1)
	{
		reboot(); //Triple panic!
	}
	va_list args;
	va_start(args,reason);
	printk("Panic: ");
	vprintk(reason,args);
	printk("ip %x\n",__builtin_return_address(0));
	printk("System halted.\n");
	status=1;
	panic("!");
	halt();
} __attribute__((noreturn));
