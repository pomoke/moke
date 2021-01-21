/*
 * This file defines error codes and common handle routines.
 */


#define OK 0
#define E_UNKNOWN 1

/* Userland only */

/* Error message routines */
struct error {
	u32 errno;
	u32 ip;
	u32 target;
};

void get_error(struct error *p);
