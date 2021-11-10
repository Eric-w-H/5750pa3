#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define BILLION 1000000000L
#define p 8

typedef struct {
    int lock_data;
    char padding[60];
} queue_entry;

typedef struct {
    simple_lock l;
    queue_entry queue[p] __attribute__ ((aligned (64)));
    short index, running;
} queue_lock;

void init( queue_lock* ql, int p )
{
    for ( size_t i = 0; i < p; i++ )
        ql->queue[i].lock_data = 0;

    ql->index = 0;
    ql->running = 0;
    s_lock_init( ql->l );
}

void lock( queue_lock* ql )
{
    s_lock( ql->l );
    short index = ql->index++;
    if ( index = p-1 ) //could prob replace this with modulo but then we can't do the cool ++ thing
        ql->index = 0;
    ql->queue[(index+1) % p].lock_data = 1;
    s_unlock( ql->l );
    while ( ql->queue[index].lock_data = 1 ) {}
    ql->running = ( ql->running + 1 ) % p;
}

void unlock( queue_lock* ql )
{
    ql->queue[ql->running].lock_data = 0;
}
