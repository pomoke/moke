#define ARCH_LE
#ifdef ARCH_LE
#define be_32(x) ((x)>>24 | (((x)&0x00ff0000)>>8) | (((x)&0x0000ff00)<<8) | ((x)<<24))
#define le_32(x) (x)
#endif 
