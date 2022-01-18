#include <type.h>
#include <io.h>
#include <kprint.h>
#include <stdarg.h>
#include <console.h>

#define PRINT_BUF_SIZE 512
static char pbuf[PRINT_BUF_SIZE];
static int kprint_target;
static char hex[]="0123456789abcdef";
static char dec[]="0123456789";

void vprintk(char *fmt,va_list args)
{
	char *p=fmt,*q=pbuf,*i,*s;
	int flag=0,c;
	char ibuf[11];
	i32 itmp;
	u32 tmp;
	i64 i64tmp;
	u64 u64tmp;
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
						  *i=hex[tmp%16];
						  tmp/=16;
					  }
					  q+=8;
					  break;
				case 'd':
					  itmp=va_arg(args,i32);
					  if (itmp<0)
					  {
						  *q++='-';
						  itmp=-itmp;
					  }
					  for (i=ibuf+10;i>=ibuf;i--)
					  {
						  *i=dec[itmp%10];
						  itmp/=10;
						  if (itmp==0)
						  	break;
					  }
					  for (;i<=ibuf+10;i++)
					  	*q++=*i;
					  break;
				case 'u':
					  tmp=va_arg(args,u32);
					  for (i=ibuf+10;i>=ibuf;i--)
					  {
						  *i=dec[tmp%10];
						  tmp/=10;
						  if (tmp==0)
							  break;
					  }
					  //Copy back
					  for (;i<=ibuf+10;i++)
						  *q++=*i;
					  break;
				case 's':
					  s=va_arg(args,char *);
					  while (*s)
						  *q++=*s++;
					  break;
				case 'c':
					  c=va_arg(args,int);
					  *q++=(char)c;
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
	
	if (console_out) //When console module was properly inited.
	{
		console_out->console_write_str(console_out,pbuf);
	}
	else
	{
		if (kprint_target==0)
			kprint(pbuf,PR_VGA,KERN);
		else
		{
			char *ptr=pbuf;
			while (*ptr)
				if (*ptr!='\n')
					serial_write(0x3f8,*ptr++);
				else
				{
					serial_write(0x3f8,'\r');
					serial_write(0x3f8,'\n');
					ptr++;
				}
		}
	}
	return;
}
void printk(char *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	vprintk(fmt,args);
	return;
}

void set_kprint_target(int a)
{
	kprint_target = a;
}