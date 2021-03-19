#include <type.h>

void clear(void *buf,u32 len)
{
	char *tmp=buf;
	for (u32 i=0;i<len;i++)
		tmp[i]=0;
	return;
}

u32 strlen(char *a)
{
	int i=0;
	while (*a++)
		i++;
	return i;
}
