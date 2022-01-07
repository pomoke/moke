/*
 * This is a type-generic double linked table functions.
 * To use this,define a struct contains `prev` and `next`.
 * The position of prev and next does not matter.
 *
 * struct some_entry {
 * 	struct some_entry *prev;
 * 	struct some_entry *next;
 * 	some_type value;
 * 	...;
 * }
 *
 */
#define DEL_ITEM(item) \
	if ((item)->prev)\
		(item)->prev->next=(item)->next;\
	if ((item)->next)\
		(item)->next->prev=(item)->prev;\

#define FREE_ITEM(item) \
	DEL_ITEM(item)\
	kfree((item));

#define ADD_ITEM(a,b,item) \
	(a)->next=(item);\
	(b)->prev=(item);\

//#define FOR_ITEM(a,b) for ( typeof((a)) b=(a) ; b->next ;b=b->next ) WRONG
#define FOR_ITEM(a,b) for ( typeof((a)) b=(a) ; b ;b=b->next )

#define FOR_APPEND(a,b)\
	for (typeof((a)) _i=(a);_i;_i->next)\
		if (!_i->next)\
		{\
			APPEND_ITEM(_i,a);\
		}

#define APPEND_ITEM(a,b)\
	(a)->next=(b);\
	(b)->prev=(a)\

#define PREV(a) ((a))->prev

#define NEXT(a) ((a))->next
