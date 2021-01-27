#define ISR_TRAP 0xf
#define ISR_INTR 0xe
extern struct idt_desc *IDT;
struct interrupt_frame
{
	u32 eip;
	u32 cs;
	u32 eflags;
};
struct idt_desc {
	u16 offset_l;
	u16 cs;
	u8 zero;
	//Control flags
	u8 type:4 ;
	u8 ss:1 ;
	u8 ring:2 ;
	u8 present:1 ;
	u16 offset_h;
} __attribute__((packed));


void intr_init(void);
void int_start(void);
void int_stop(void);
void int_ack(void);
void idt_write(struct idt_desc *idt,u32 func,u8 type,u8 ring);
