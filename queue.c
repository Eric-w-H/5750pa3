#include "lock-q.h"

#include <stdio.h>

void lock_init( struct queue_lock* ql )
{
    ql->queue[0].lock_data = 0;
    for ( int i = 1; i < P; i++ )
    {
        ql->queue[i].lock_data = 0;
    }

    ql->index = 0;
    ql->running = 0;
    s_lock_init( &ql->l );
}


void lock( struct queue_lock* ql )
{
    s_lock( &ql->l );

    //could prob replace this with modulo but then we can't do the cool ++ thing
    uint8_t index = ql->index++;
    if ( index == P-1 ) 
        ql->index = 0;

    s_unlock( &ql->l );
    
    while ( ql->queue[index].lock_data == 1 ) { }
}


void unlock( struct queue_lock* ql )
{

    ql->queue[ql->running].lock_data = 1;
    ql->running++;
    if(ql->running >= P) ql->running = 0;
    ql->queue[ql->running].lock_data = 0;
}
