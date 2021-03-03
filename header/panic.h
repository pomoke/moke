void panic(char * fmt,...) __attribute__ ((noreturn));

#define BUG_ON(x) \
       	if ((x))\
	{\
		panic("BUG: %s line %d,BUG_ON(%s)\n",__FILE__,__LINE__,#x);\
	}\
