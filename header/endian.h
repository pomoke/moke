#ifdef ARCH_LE
#define be_32(x) ((x)>>24 | ((x)>>8) | ((x)<<8) | ((x)<<24))
#define le
#endif 
