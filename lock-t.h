#pragma once

struct simple_lock {
    short lock_data; 
};

struct ticket_lock {
    struct simple_lock l;
    short next_available;
    short currently_serving;
};

extern void s_lock_init(struct simple_lock *lkp);
extern void s_lock(struct simple_lock *lkp);
extern void s_lock_try(struct simple_lock *lkp);
extern void s_unlock(struct simple_lock *lkp);

void lock_init(struct ticket_lock*);
void lock(struct ticket_lock*);
void unlock(struct ticket_lock*);
