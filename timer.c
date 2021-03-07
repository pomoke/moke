/*
 *  Timer and Clock meta-driver for MOKE.
 *  
 *  A arch/platform-specific backend driver (with arch_ prefix) is required.
 *
 */

#include <type.h>
#include <kprint.h>
#include <mem.h>
#include <panic.h>
#include <list.h>

struct time_spec {
	u32 sec;
	u32 nsec; //Senseless for now.
};
u64 ticks;
u64 time;
/*
 * 4-level timing wheel. 
 * 100ms - 10s - 1000s - 10*6 s
 *
 */
struct timer {
	struct time_spec time;
	struct time_spec interval;
	void (*func)();
	int type;
	struct timer_item *prev;
	struct timer_item *next;
};
/* 
 * timer.type schema:
 * bit 0: Enable bit
 * - bit 1: Periodical bit(interval)
 *
 */


struct timer_item {
	struct timer *timer;
	struct timer_item *prev;
	struct timer_item *next;
};


#define TIMER_MAGIC 0x632f632f

struct time_layer {
	u32 unit; //in microseconds.
	struct timer_list *i[100];
	int now;
} wheel[4];

struct timer_backend {
	u32 (*)(struct timespec *) tick_set;
	struct timespec *(*)(struct timespec *) time_get;
	u32 tick_perc;
	u32 time_perc;
	u32 type;
};

struct timer *timer_create(void)
{
	struct timer *p=kalloc(sizeof(timer));
	p->prev=NULL;
	p->next=NULL;
	return p;
}

int timer_set(struct timer *t,struct time_spec *time,void (*)(void) func) 
{
	;
}


int timer_pause(struct timer *t)
{
	;
}

int periodical_set(struct timer *t,struct time_spec *interval,void (*)(void) func)
{
	t.interval=*interval;
	return 1;
}

int timer_stop(struct timer *t)
{
	//Set target time to 0.
	;
}

int timer_del(struct timer *t)
{
	//Remove from linked-list.
	if (t->next)
		t->next->prev=t->prev;
	if (t->prev)
		t->prev->next=t->next;
	kfree(t);
}

void timer_handler(void)
{
	//timing-wheel handler.
	
	//Check the wheel for deadlines.
	struct timer_item *p=wheel->timer_list[wheel->now];
	for (;p=p->next;p)
	{
		p->func(); //Callback.
		if (p->interval.sec || p->interval.usec)
			//Re-insert timer.
			;
		else
		{
			//Invalidate timer.
			;
		}
	}
	wheel->now++;
	if (wheel->now>=100)
		wheel->now=0;

	//Check upper wheels.
	for (i=1;i<4;i++)
	{
		//If a deadline is not longer than lower wheel,the we move it.
		FOR_ITEM(wheel[i].timer_list[wheel[i].now],i)
		{
			;
		}
		//Increase 'now' value when needed.
		if (!((ticks*TICK_MS)%wheel[i].interval))
			wheel[i].now++;

	}
	return;
	
}
void tick_handler(void) //Called in interrupt handler. No scheduling allowed.
{
	tick++;
}

void clock_handler(void) //For non-periodical timer shots.
{
	;
}

void timer_init(void)
{
	wheel[0].interval=100;
	wheel[1].interval=10000;
	wheel[2].interval=1000000;
	wheel[3].interval=100000000;
	return ;
}
