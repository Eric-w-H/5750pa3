#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "lock.h"
#define BILLION 1000000000L
#define N 500000000

typedef struct {
    struct timespec start, end;
    int* a;
    int pid, k, m;
    struct simple_lock* lock;
} GM;


//-----------------------------------------------------------------
// time_less_than
//-----------------------------------------------------------------
int time_less_than(struct timespec* a, struct timespec* b)
{
    if(a->tv_sec > b->tv_sec) return 0;
    if(a->tv_sec < b->tv_sec) return 1;
    if(a->tv_nsec < b->tv_nsec) return 1;
    return 0;
}


//-----------------------------------------------------------------
// time_greater_than
//-----------------------------------------------------------------
int time_greater_than(struct timespec* a, struct timespec* b)
{
    if(a->tv_sec < b->tv_sec) return 0;
    if(a->tv_sec > b->tv_sec) return 1;
    if(a->tv_nsec > b->tv_nsec) return 1;
    return 0;
}


//-----------------------------------------------------------------
// Ubench
//-----------------------------------------------------------------
// the PA ubench code
void* ubench(void* parm)
{
    GM* arg = (GM*) parm;
    int p=0, q=0;
    clock_gettime(CLOCK_MONOTONIC, &arg->start);
    for(int i = 0; i < N; ++i)
    {
        s_lock(arg->lock);
        for(int j = 0; j < arg->k; ++j) ++q;
        s_unlock(arg->lock);
        for(int j = 0; j < arg->m; ++j) ++p;
    }
    clock_gettime(CLOCK_MONOTONIC, &arg->end);
    arg->a[arg->pid] = p + q;
    return NULL;
}


void dispatch_threads_m_const(int m, int k)
{
    //-----------------------------------------------------------------
    // Setup
    //-----------------------------------------------------------------
    const int p = 8;
    pthread_t *threads = malloc((p-1) * sizeof(threads));
    GM *args = malloc(p * sizeof(*args));
    int* a = malloc(p * sizeof(int));

    //-----------------------------------------------------------------
    // Execution
    //-----------------------------------------------------------------  
    // Assign work to each pthread, and create it,
    // spawning the thread
    struct timespec start, end;
    struct simple_lock l;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    s_lock_init(&l);

    for(int i = 0; i < p-1; i++) {
        args[i].a = a;
        args[i].k = k;
        args[i].m = m;
        args[i].pid = i;
        args[i].lock = &l;
        pthread_create(&threads[i], NULL, ubench, &args[i]);
    }

    args[p-1].a = a;
    args[p-1].k = k;
    args[p-1].m = m;
    args[p-1].pid = p-1;
    args[p-1].lock = &l;
    ubench((void*)&args[p-1]);

    for(int i = 0; i < p-1; ++i){
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    //-----------------------------------------------------------------
    // Output
    //-----------------------------------------------------------------
    struct timespec *thread_start=&args[0].start, *thread_end=&args[0].end;
    for(int i = 1; i < p; ++i)
    {
        if(time_greater_than(thread_start, &args[i].start)) thread_start = &args[0].start;
        if(time_less_than(thread_end, &args[i].end)) thread_end = &args[0].end;
    }

    double total_time = BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    total_time = total_time / BILLION;

    double thread_time = BILLION *(thread_end->tv_sec - thread_start->tv_sec) +(thread_end->tv_nsec - thread_start->tv_nsec);
    thread_time = thread_time / BILLION;
    
    printf("%lf,%lf\n", total_time, thread_time);
    
    //-----------------------------------------------------------------
    // Cleanup
    //-----------------------------------------------------------------
    free(args);
    free(a);
}


void dispatch_threads_m_prop(int m, int k)
{
    //-----------------------------------------------------------------
    // Setup
    //-----------------------------------------------------------------
    const int p = 8;
    pthread_t *threads = malloc((p-1) * sizeof(threads));
    GM *args = malloc(p * sizeof(*args));
    int* a = malloc(p * sizeof(int));

    //-----------------------------------------------------------------
    // Execution
    //-----------------------------------------------------------------  
    // Assign work to each pthread, and create it,
    // spawning the thread
    struct timespec start, end;
    struct simple_lock l;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    s_lock_init(&l);

    for(int i = 0; i < p-1; i++) {
        args[i].a = a;
        args[i].k = k;
        args[i].m = m * i;
        args[i].pid = i;
        args[i].lock = &l;
        pthread_create(&threads[i], NULL, ubench, &args[i]);
    }

    args[p-1].a = a;
    args[p-1].k = k;
    args[p-1].m = m * (p - 1);
    args[p-1].pid = p-1;
    args[p-1].lock = &l;
    ubench((void*)&args[p-1]);

    for(int i = 0; i < p-1; ++i){
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    //-----------------------------------------------------------------
    // Output
    //-----------------------------------------------------------------
    struct timespec *thread_start=&args[0].start, *thread_end=&args[0].end;
    for(int i = 1; i < p; ++i)
    {
        if(time_greater_than(thread_start, &args[i].start)) thread_start = &args[0].start;
        if(time_less_than(thread_end, &args[i].end)) thread_end = &args[0].end;
    }

    double total_time = BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    total_time = total_time / BILLION;

    double thread_time = BILLION *(thread_end->tv_sec - thread_start->tv_sec) +(thread_end->tv_nsec - thread_start->tv_nsec);
    thread_time = thread_time / BILLION;
    
    printf("%lf,%lf\n", total_time, thread_time);
    
    //-----------------------------------------------------------------
    // Cleanup
    //-----------------------------------------------------------------
    free(threads);
    free(args);
    free(a);
}


//-----------------------------------------------------------------
// Main
//-----------------------------------------------------------------
int main(int argc, char** argv)
{
    //-----------------------------------------------------------------
    // Parse command line
    //-----------------------------------------------------------------
    if(argc != 2) {
        printf("Usage: %s k\nAborting...\n", argv[0]);
        exit(0);
    }

    int k = atoi(argv[1]);

    if(k < 0){
        printf("k must be greater than 0, aborting...\n");
        exit(0);
    }

    printf("total_time,thread_time\n");
    dispatch_threads_m_const(0, k);
    dispatch_threads_m_const(1000, k);
    dispatch_threads_m_prop(500, k);

    return 0;
}
