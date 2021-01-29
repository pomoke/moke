#include <type.h>
#include <io.h>
#include <link.h>
#include <intr.h>
/* 
 * 8250 serial driver
 * Currently we use 8n1 only.
 *
 */

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

#define DATA(x) (x)
#define INT(x) ((x)+1)
#define ID(x) ((x)+2)
#define LINE_CTRL(x) ((x)+3)
#define MODEM_CTRL(x) ((x)+4)
#define LINE_STAT(x) ((x)+5)
#define MODEM_STAT(x) ((x)+6)
#define SCRATCH(x) ((x)+7)

//With DLAB as 1
#define DIV_L(x) (x)
#define DIV_H(x) ((x)+1)

#define BASE_HZ 115200 
/*The maximum frequency supported. DO NOT use a zero divisor!*/

void isr serial_handler(struct interrupt_frame *frame);


int uart_set_baud(u32 port,u32 baud)
{
	printk("Starting serial port...\n");
	outb(INT(port),0x00); //Stop interrupts
	u32 divisor=BASE_HZ/baud;
	outb(LINE_CTRL(port),0x80); //Enable DLAB
	outb(DIV_L(port),divisor&0xff); //Low byte
	outb(DIV_H(port),(divisor&0xff00)>>8); //High byte
	outb(LINE_CTRL(port),0x03); //8n1,no parity
	outb(ID(port),0xc7);
	outb(MODEM_CTRL(port),0x0b);
	//outb(MODEM_CTRL(port),0xae); //Set lookback
	//outb(port,0xae); //Test output
	//if (inb(port)!=0xae)
	//	return 1;

	outb(MODEM_CTRL(port),0x0f);
	outb(INT(port),0x1);
	//check intr id
	idt_write(IDT+0x20+6,serial_handler,ISR_INTR,0);
	while(serial_received(port));
	printk("started serial port at baud %d\n",baud);
	return 0;
}

int serial_received(u32 port)
{
	return inb(LINE_STAT(port))&1;
}

int serial_read(u32 port)
{
	while (!serial_received(port)) ;
	return inb(port);
}

int serial_checkbuf(u32 port)
{
	return inb(LINE_STAT(port))&0x20;
}

void serial_write(u32 port,u8 a)
{
	while (!serial_checkbuf(port));
	outb(port,a);
	return;
}

isr void serial_handler(struct interrupt_frame *frame)
{
	char a;
	kprint("serial\n");
	while (serial_received(0x3f8))
	{
		a=serial_read(0x3f8);
		serial_write(0x3f8,a);
	}
	int_ack();
	return;
}
