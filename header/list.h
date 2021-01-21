/*
 * This is a type-agnostic double linked table functions.
 * To use this,implent a struct contains `prev` and `next`.
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


