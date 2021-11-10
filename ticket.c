#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define BILLION 1000000000L

typedef struct {
    short lock_data; 
} simple_lock;

typedef struct {
    simple_lock l;
    short next_available;
    short currently_serving
} ticket_lock;

void init( ticket_lock* tl )
{
    s_lock_init( &tl->l );
    tl->next_available = 0;
    tl->currently_serving = 0;
}

void lock( ticket_lock* tl )
{
    s_lock( tl->l ); 
    short position = tl->next_available++;
    s_unlock( tl->l );
    while ( tl->currently_serving < position ) {}
}

void unlock( ticket_lock* tl )
{
    tl->currently_serving++;
}
