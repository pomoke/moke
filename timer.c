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
#include <string.h>
#define S_PER_NS  1000000000
#define MS_PER_NS 1000000
#define US_PER_NS 1000

#define TICK_MS 10
struct time_spec {
	u32 sec;
	i32 nsec; //Senseless for now.
};
struct time_spec now;
u32 ticks;
u32 epoch; //Time starts from 1970.
/*
 * 4-level timing wheel. 
 * 100ms - 10s - 1000s - 10*6 s
 *
 */
struct time_spec time_diff(struct time_spec *a,struct time_spec *b)
{
	struct time_spec ret;
	ret.sec=a->sec-b->sec;
	ret.nsec=a->nsec-b->nsec;
	if (ret.nsec / S_PER_NS )
	{
		ret.sec		+=ret.nsec / S_PER_NS;
		ret.nsec	 =ret.nsec % S_PER_NS;
	}
	return ret;
}

u32 time_diff_ms(struct time_spec *a,struct time_spec *b)
{
	return (a->sec-b->sec)*1000+(a->nsec-b->nsec)/MS_PER_NS;
}

void time_add_ms(struct time_spec *dst,u32 msec)
{
	dst->nsec+=msec*MS_PER_NS;
	if (dst->nsec / S_PER_NS)
	{
		dst->sec += dst->nsec / S_PER_NS;
		dst->nsec = dst->nsec % S_PER_NS;
	}
	return ;
}

struct time_spec time_add(struct time_spec *a,struct time_spec *b)
{
	struct time_spec ret;
	ret->sec=a->sec+b->sec;
	ret->nsec=a->nseb->nsec;
	if (ret->nsec / S_PER_NS)
	{
		ret->sec	+= ret->nsec / S_PER_NS;
		ret->nsec	=  ret->nsec % S_PER_NS;
	}
	return ret;
}
struct timer {
	struct time_spec time;
	struct time_spec interval;
	void (*func)();
	int type;
	struct timer *prev;
	struct timer *next;
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
	struct timer *timer_list[100];
	int now;
} wheel[4];

struct timer_backend {
	u32 (*tick_set)(struct time_spec *);
	struct time_spec *(*time_get)(struct time_spec *);
	u32 tick_perc;
	u32 time_perc;
	u32 type;
};

struct timer *timer_create(void)
{
	struct timer *p=kalloc(sizeof(struct timer),0);
	//p->prev=NULL;
	//p->next=NULL;
	clear(p,sizeof(struct timer));
	return p;
}

int timer_insert(struct timer *t)
{
	//Check the right position to insert.
	for (int i=3;i<=0;i--)
	{
		if (time_diff_ms(&t->time,&now)<wheel[i].unit)
			continue;
		else
		{
			//Append to the wheel list.
			int where = time_diff_ms(&t->time,&now)/wheel[i].unit > 99 ? 99 : time_diff_ms(&t->time,&now)/wheel[i].unit;
			if (!wheel[i].timer_list[where])
			{
				wheel[i].timer_list[where]=t;
			}
			else
				FOR_ITEM(wheel[i].timer_list[where],j)
				{
					if (!j->next)
					{
						APPEND_ITEM(j,t);
						break;
					}
				}
		}
	}
}


int timer_set(struct timer *t,struct time_spec *time,void (*func)(void) ) 
{
	t->time=*time;
	t->func=func;
	timer_insert(t);
}

int timer_at(struct timer *t,struct time_spec *time,void (*func)(void))
{
	t->time=*time;
	t->func=func;
	timer_insert(t);
}

int timer_pause(struct timer *t)
{
	//Remove from timing-wheel. 
	DEL_ITEM(t);
}

int periodical_set(struct timer *t,struct time_spec *interval,void (*func)(void))
{
	t->time=time_add(&now,interval);
	t->interval=*interval;
	return 1;
}

int timer_stop(struct timer *t)
{
	//Set target time to 0.
	DEL_ITEM(t);
	t->time.sec=0;
	t->time.nsec=0;
	t->interval.sec=0;
	t->interval.nsec=0;
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
	//struct timer_item *p=wheel->timer_list[wheel->now];
	for (int i=0;i<=TICK_MS-1;i++)
	{
		for (struct timer *p=wheel->timer_list[wheel->now+i>=100 ? wheel->now+i-99 : wheel->now+i];p;p=p->next)
		{
			p->func(); //Do callback.
			if (p->interval.sec || p->interval.nsec)
				//Re-insert timer.
				timer_set(p,&p->interval,p->func);
			else
			{
				//Invalidate timer.
				timer_stop(p);
			}
		}
	}
	wheel->now++;
	if (wheel->now>=100)
		wheel->now=0;
	//Check upper wheels.
	for (int i=1;i<4;i++)
	{
		struct timer *k;
		//If a deadline is not longer than lower wheel,the we move it.
		FOR_ITEM(wheel[i].timer_list[wheel[i].now],j)
		{
			if (!j)
				break;
			k=j;
			if (time_diff_ms(&k->time,&now)<wheel[i].unit) //Problem here.
			{
				DEL_ITEM(k)
				//Append
				int where=(time_diff_ms(&k->time,&now)/wheel[i-1].unit+wheel[i-1].now);
				if (where>=100)
					where-=100;
				if (!wheel[i-1].timer_list[where])
				{
					wheel[i-1].timer_list[where]=k;
				 	k->prev=NULL;
					k->next=NULL;
				}
				else
				{
					FOR_ITEM(wheel[i-1].timer_list[where],tmp)
					{
						if (!tmp->next)
						{
							tmp->next=k;
							k->prev=tmp;
							k->next=NULL;
						}
					}
				}
			}
		}
		//Increase 'now' value when needed.
		if (!((ticks*TICK_MS)%wheel[i].unit))
		{
			wheel[i].now++;
			if (wheel[i].now>=100)
				wheel[i].now=0;
		}

	}
	return;
	
}
void tick_handler(void) //Called in interrupt handler. No scheduling allowed.
{
	ticks++;
	time_add_ms(&now,TICK_MS);
	timer_handler();
}

void clock_handler(void) //For non-periodical timer shots. Currently unavailable on x86.
{
	return;
}

void timer_init(void)
{
	wheel[0].unit=100;
	wheel[1].unit=10000;
	wheel[2].unit=1000000;
	wheel[3].unit=100000000;
	return ;
}
