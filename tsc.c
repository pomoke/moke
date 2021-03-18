*
 * TSC clock support for MOKE.
 *
 */
#include <type.h>
#include <cpuid.h>

u64 tsc_read(void)
{
	u64 tick;
	asm volatile("rdtsc":"=A"(tick)::);
	//For x86-64 : should be spilted.  
	return tick;
}

int tsc_init(void)
{
	//Check TSC availability
	if (!cpuid_feature(CPUID_TSC))
		return 0;
	else
	{
		//Return a timer struct.
		return 1;
	}
}
