#include "header/type.h"
#include "header/io.h"
#include "header/kprint.h"
#include "header/intr.h"
#include "header/cmos.h"
#include <bcd.h>
#include <string.h>

//RTC wall clock
struct date {
	u16 year;
	u8 month;
	u8 day;
	u8 weekday;
	u8 hour;
	u8 minute;
	u8 second;
};

char *week[]={"Sunday\n","Monday\n","Tuesday\n","Wednesday\n",
	"Thursday\n","Friday\n","Saturday\n"};
void rtc_init(void)
{
	int_stop();
	outb(0x70,0x8B);
	u8 prev=inb(0x71);
	outb(0x70,0x8b);
	outb(0x71,prev | 0x40);
	int_start();
	return;
}
struct date * raw_rtc_read(struct date * this) //Assume we are in the 21th century!
{
	this->year	=cmos_read(0x09);
	this->month	=cmos_read(0x08);
	this->day	=cmos_read(0x07);
	this->weekday	=cmos_read(0x06);
	this->hour	=cmos_read(0x04);
	this->minute	=cmos_read(0x02);
	this->second	=cmos_read(0x00);
	return this;
}
struct date * rtc_read(struct date * this) //Assume we are in the 21th century!
{
	raw_rtc_read(this);
	this->year	=BCD_TO_INT(this->year)+2000;
	this->month	=BCD_TO_INT(this->month);
	this->day	=BCD_TO_INT(this->day);
	this->weekday	=BCD_TO_INT(this->weekday);
	this->hour	=BCD_TO_INT(this->hour);
	this->minute	=BCD_TO_INT(this->minute);
	this->second	=BCD_TO_INT(this->second);

	return this;
}

void clock_now(void)
{
	struct date now;
	clear(&now,sizeof(now));
	rtc_read(&now);
	printk("%d-%d-%d %d:%d:%d\n",now.year,now.month,now.day,now.hour,now.minute,now.second);
	return;
}
