#define BCD_TO_INT(x) ((((x)>>4)&0xf)*10+(x)&0xf)
//#define BCD_TO_INT(x) (((x&0xf0)>>1)+((bcd&0xf0)>>3)+(bcd&0xf))
