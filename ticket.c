#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define BILLION 1000000000L

typedef struct {
    short lock_data; 
} simplelock;

typedef struct {
    simplelock* l;
    short next_available;
    short currently_serving
} ticket_lock;

void ticket_init( ticket_lock* tl )
{
    tl->l = malloc( sizeof( simplelock ) );
    s_lock_init( tl->l );
    tl->next_available = 0;
    tl->currently_serving = 0;
}

void ticket_acquire( ticket_lock* tl )
{
    s_lock( tl->l );
    position = tl->next_available++;
    s_unlock( tl->l );
    while ( tl->currently_serving < position ) {}
}

void ticket_unlock( ticket_lock* tl )
{
    tl->currently_serving++;
}

    
    
