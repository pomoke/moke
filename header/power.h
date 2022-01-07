void reboot(void);
void halt(void);
void reboot_tflt(void);

struct power *power_init(struct power *);

struct power_control {
    int how;
};

struct power {
    void (*power_off)(void);
    void (*reboot)(void);
    void (*halt)(void);
    int (*set)(struct power_control *p);
    int (*get)(struct power_control *p);
};