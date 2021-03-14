//Kernel log infrasture.
#include <type.h>
#include <panic.h>
enum {
	KLOG_APPEND=1;
};

void klogctl(int op,char *buf,u32 len)
{
	;
}

void klog(char *log)
{
	;
}
