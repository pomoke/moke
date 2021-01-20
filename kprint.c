#include "header/type.h"
#include "header/io.h"
#include "header/kprint.h"
#include <stdarg.h>
#define PRINT_BUF_SIZE 512
static char pbuf[PRINT_BUF_SIZE];
void vprintk(char *fmt,va_list args)
{
	char *p=fmt,*q=pbuf,*i;
	int flag=0;
	char ibuf[11];
	u32 tmp;
	while (*p)
	{
		if (flag==1)
		{
			switch (*p)
			{
				case '%': *q++='%';break;
				case 'x': 
				case 'p':
					  tmp=va_arg(args,u32);
					  for (i=q+7;i>=q;i--)
					  {
						  *i="01234567890abcdef"[tmp%16];
						  tmp/=16;
					  }
					  q+=8;
					  break;
				case 'd':
					  tmp=va_arg(args,u32);
					  for (i=ibuf+10;i>=ibuf;i--)
					  {
						  *i="0123456789"[tmp%10];
						  tmp/=10;
						  if (tmp==0)
							  break;
					  }
					  //Copy back
					  for (;i<=ibuf+10;i++)
						  *q++=*i;
					  break;
				default:
					  *q++='%';
					  *q++=*p;

			}
			flag=0;
		}
		else if (*p=='%')
			flag=1;
		else
			*q++=*p;
		p++;
	}
	*q='\0';
	kprint(pbuf,PR_VGA,KERN);
	return;
}
void printk(char *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	vprintk(fmt,args);
	return;
}

