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
			p++;
			q++;
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

char *strstr(char * haystack, char * needle){
    int haystack_len = strlen(haystack);
    int ret=0;
    int haystack_pos=0,needle_pos=0;
    int needle_len = strlen(needle);
    int move[27];
    if (needle_len > haystack_len)
    {
	    return -1;
    }
    if (needle_len == 0 )
    {
	    return 0;
    }
    for (int i=0;i<26;i++)
    {
        move[i]=-1;
    }
    for (int i=needle_len-1;i>=0;i--)
    {
        if (move[needle[i]-'a']==-1)
        {
            move[needle[i]-'a']=needle_len-i;
        }
    }
    for (int i=0;i<26;i++)
	    if (move[i]==-1)
		    move[i]=needle_len+1;
    while (haystack_pos < haystack_len && haystack_pos + needle_len-1 < haystack_len)
    {
        needle_pos = 0;
        while (needle_pos<needle_len && haystack_pos + needle_len-1  < haystack_len)
        {
            if (needle[needle_pos]!=haystack[haystack_pos+needle_pos])
            {
		if (haystack_pos+needle_len < haystack_len)
		{
                	haystack_pos += move[haystack[haystack_pos+needle_len]-'a'];
	                break;
		}
		else goto end;
            }
            else
            {
                needle_pos++;
            }
        }
        if (needle_pos==needle_len)
        {
            break;
        }
    }
end:
	ret = (needle_pos == needle_len) ? haystack_pos : -1;
    return (ret == -1) ? (char *)0 : haystack_pos+ret;
}