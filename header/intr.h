#define isr __attribute__((interrupt,target(general-regs-only)))
void intr_init(void);
void int_start(void);
void int_stop(void);
void int_ack(void);
