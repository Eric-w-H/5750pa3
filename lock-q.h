#pragma once
#define P 8

struct simple_lock {
    short lock_data; 
};

struct queue_entry {
    int lock_data;
    char padding[60];
};

struct queue_lock {
    struct simple_lock l;
    struct queue_entry queue[P] __attribute__ ((aligned (64)));
    short index, running;
};

extern void s_lock_init(struct simple_lock *lkp);
extern void s_lock(struct simple_lock *lkp);
extern void s_lock_try(struct simple_lock *lkp);
extern void s_unlock(struct simple_lock *lkp);

void lock_init(struct queue_lock*);
void lock(struct queue_lock*);
void unlock(struct queue_lock*);
