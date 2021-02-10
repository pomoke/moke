/*
 * This is a type-generic double linked table functions.
 * To use this,define a struct contains `prev` and `next`.
 * The position of prev and next does not matter.
 *
 * struct some_entry {
 * 	struct some_entry *prev;
 * 	struct some_entry *next;
 * 	some_type value;
 * }
 *
 */
#define DEL_ITEM(item) \
	(item)->prev->next=(item)->next;\
	(item)->next->prev=(item)->prev;\

#define FREE_ITEM(item) \
	DEL_ITEM(item)\
	kfree((item));

#define ADD_ITEM(a,b,item) \
	(a)->next=(item);\
	(b)->prev=(item);\

//#define FOR_ITEM(a,b) for ( typeof((a)) b=(a) ; b->next ;b=b->next ) WRONG!
#define FOR_ITEM(a,b) for ( typeof((a)) b=(a) ; b ;b=b->next )



#define PREV(a) ((a))->prev

#define NEXT(a) ((a))->next
