#include "lock.h"

void lock( struct simple_lock* tl ){ s_lock(tl); }

void lock_init( struct simple_lock* tl ){ s_lock_init(tl); }

void unlock( struct simple_lock* tl ){ s_unlock(tl); }
