#include <type.h>
#include <io.h>
#include <intr.h>
#include <error.h>

//All are 8-bit ports
#define PIT_FREQ 1193182 //Terrible frequency value indeed.
#define PIT_0 0x40 //IRQ ticks
#define PIT_1 0x41 //Unused
#define PIT_2 0x42 //PC speaker
#define PIT_MODE 0x43 //Write only

struct pit_mode {
	u8 bcd:1; /* BCD/Binary mode. */
	u8 mode:2;
	u8 access:2;
	u8 channel:2;
};

struct pit_mode pit_ops;


