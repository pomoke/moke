#include <type.h>
#include <io.h>
#include <intr.h>
#include <link.h>

/* 
 * Driver of i8042 keybord controller
 */
//Ports
#define KBD_DATA 0x60
#define KBD_STAT 0x64 //read
#define KBD_CMD 0x64 //write

//Command bytes
#define KBD_LED 0xed
#define KBD_ECHO 0xee
#define KBD_SCANSET 0xf0
#define KBD_ID 0xf2
#define KBD_TYPEMATIC 0xf3
#define KBD_START_SCAN 0xf4
#define KBD_STOP_SCAN 0xf5
#define KBD_DEFAULT 0xf6
#define KBD_SET_TYPEMATIC 0xf7
#define KBD_MAKE_RELEASE 0xf8
#define KBD_MAKE_ONLY 0xf9
#define KBD_TAMR 0xfa
#define KBD_REPEAT 0xfe
#define KBD_RESET 0xff

//Response bytes
#define KBD_ACK 0xfa
#define KBD_RESEND 0xfe
#define KBD_TEST_OK 0xaa

//i8042 kbd controller uses int 2.

void isr i8042_entry(struct interrupt_frame)
{
	return;
}
void i8042_init(void)
{
	return;
}

void i8042_stop(void)
{
	;
	return;
}
