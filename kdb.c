#include <type.h>
#include <io.h>
#include <serial.h>
#include <mem.h>

//Simple Kernel Debugger which does nothing.
//Maybe a GDB stub works better.


char cmdbuf[256];

void start_kdb(void) //Call this on panic or `break`.
{
	;
}

