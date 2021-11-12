#pragma once
#define P 8
#include <stdint.h>

struct simple_lock {
    int lock_data; 
};

struct queue_entry {
    volatile uint8_t lock_data;
    uint8_t padding[63];
};

struct queue_lock {
    struct simple_lock l;
    struct queue_entry queue[P] __attribute__ ((aligned (64)));
    uint8_t index;
    uint8_t running;
};

extern void s_lock_init(struct simple_lock *lkp);
extern void s_lock(struct simple_lock *lkp);
extern void s_lock_try(struct simple_lock *lkp);
extern void s_unlock(struct simple_lock *lkp);

void lock_init(struct queue_lock*);
void lock(struct queue_lock*);
void unlock(struct queue_lock*);
