#include "lock-q.h"

void lock_init( struct queue_lock* ql )
{
    for ( int i = 0; i < P; i++ )
        ql->queue[i].lock_data = 0;

    ql->index = 0;
    ql->running = 0;
    s_lock_init( &ql->l );
}

void lock( struct queue_lock* ql )
{
    s_lock( &ql->l );
    short index = ql->index++;
    if ( index == P-1 ) //could prob replace this with modulo but then we can't do the cool ++ thing
        ql->index = 0;
    ql->queue[(index+1) % P].lock_data = 1;
    s_unlock( &ql->l );
    while ( ql->queue[index].lock_data == 1 ) {}
    ql->running = ( ql->running + 1 ) % P;
}

void unlock( struct queue_lock* ql )
{
    ql->queue[ql->running].lock_data = 0;
}
