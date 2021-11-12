#include "lock-t.h"

void lock_init( struct ticket_lock* tl )
{
    s_lock_init( &tl->l );
    tl->next_available = 0;
    tl->currently_serving = 0;
}

void lock( struct ticket_lock* tl )
{
    s_lock( &tl->l ); 
    short position = tl->next_available++;
    s_unlock( &tl->l );
    while ( tl->currently_serving < position ) {}
}

void unlock( struct ticket_lock* tl )
{
    tl->currently_serving++;
}
