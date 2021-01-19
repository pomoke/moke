//RTC wall clock
struct date {
	u8 year;
	u8 month;
	u8 day;
	u8 weekday;
	u8 hour;
	u8 minute;
	u8 second;
};
extern char *week[];
void rtc_init(void);
struct date * rtc_read(struct date * this);//Assume we are in the 21th century!
