#pragma once

struct simple_lock {
  int lock_data;
};

extern void s_lock_init(struct simple_lock *lkp);
extern void s_lock(struct simple_lock *lkp);
extern void s_lock_try(struct simple_lock *lkp);
extern void s_unlock(struct simple_lock *lkp);

void lock_init(struct simple_lock*);
void lock(struct simple_lock*);
void unlock(struct simple_lock*);
