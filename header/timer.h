#define S_PER_NS  1000000000
#define MS_PER_NS 1000000
#define US_PER_NS 1000
#define TICK_MS 10
struct time_spec {
	u32 sec;
	i32 usec;
};

struct timer {
	struct time_spec time;
	struct time_spec interval;
	void (*func)();
	int type;
	struct timer *prev;
	struct timer *next;
};

extern struct time_spec now;
extern u32 ticks,epoch;


struct time_spec time_diff(struct time_spec *a,struct time_spec *b);
u32 time_diff_ms(struct time_spec *a,struct time_spec *b);
void time_add_ms(struct time_spec *dst,u32 msec);
struct time_spec time_add(struct time_spec *a,struct time_spec *b);
struct time_spec *timer_create(void);

int timer_set(struct timer *t,struct time_spec *time,void (*func)(void));
int periodical_set(struct timer *t,struct time_spec *interval,void (*func)(void))
int timer_at(struct timer *t,struct time_spec *time,void (*func)(void));
int timer_stop(struct timer *t);
int timer_stop(struct timer *t);
int timer_del(struct timer *t);

void timer_handler(void);
void tick_handler(void);
void clock_handler(void);
void timer_init(void);

void timer_test(void);
