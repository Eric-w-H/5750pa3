#pragma once
#include <stdint.h>

struct simple_lock {
    int lock_data; 
};

struct ticket_lock {
    struct simple_lock l;
    uint8_t next_available;
    volatile uint8_t currently_serving;
};

extern void s_lock_init(struct simple_lock *lkp);
extern void s_lock(struct simple_lock *lkp);
extern void s_lock_try(struct simple_lock *lkp);
extern void s_unlock(struct simple_lock *lkp);

void lock_init(struct ticket_lock*);
void lock(struct ticket_lock*);
void unlock(struct ticket_lock*);
