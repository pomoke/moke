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

int strcmp(char *a,char *b)
{
	char *p=a,*q=b;
	int ret=0;
	while(*p && *q)
	{
		if (*p<*q)
		{
			ret=-1;
			goto end;
		}
		else if (*p>*q)
		{
			ret=1;
			goto end;
		}
		else
		{
			*p++;
			*q++;
		}
	}
	if (p-a<q-b)
		ret=-1;
	else if (p-a==q-b)
		ret=0;
	else 
		ret=1;

end:
	return ret;
}

u32 strequ(char *a,char *b)
{
	char *p=a,*q=b;
	while (*p && *q)
	{
		if (*p++ != *q++)
			break;
	}
	return (*p==*q) ? 1 : 0 ; 
}
