#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "lock-q.h"
#include "N.h"

typedef struct {
    struct timespec start, end;
    int* a;
    int pid, k, m, p, q;
    struct queue_lock* lock;
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

void check_arr(struct queue_entry* e)
{
  int s = 8;
  for(int i = 0; i < P; ++i)
    s -= e[i].lock_data;
  // printf("Number of open slots: %d\n", s);
  if(s!=1) exit(-1);
}


//-----------------------------------------------------------------
// Ubench
//-----------------------------------------------------------------
// the PA ubench code
void* ubench(void* parm)
{
    GM* arg = (GM*) parm;
    arg->p=0;
    arg->q=0;
    clock_gettime(CLOCK_MONOTONIC, &arg->start);
    for(int i = 0; i < N; ++i)
    {
        lock(arg->lock);
        for(int j = 0; j < arg->k; ++j) {++arg->q;}

        // check_arr(arg->lock->queue);

        unlock(arg->lock);
        for(int j = 0; j < arg->m; ++j) {++arg->p;}

        // if(i % 100000 == 0)
        //   printf("Iter %d, pid %d\n", i, arg->pid);
    }
    clock_gettime(CLOCK_MONOTONIC, &arg->end);
    arg->a[arg->pid] = arg->p + arg->q;
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
    struct queue_lock l;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    lock_init(&l);

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
        if(time_greater_than(thread_start, &args[i].start)) thread_start = &args[i].start;
        if(time_less_than(thread_end, &args[i].end)) thread_end = &args[i].end;
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
    struct queue_lock l;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    lock_init(&l);

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
        if(time_greater_than(thread_start, &args[i].start)) thread_start = &args[i].start;
        if(time_less_than(thread_end, &args[i].end)) thread_end = &args[i].end;
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
    if(argc != 3) {
        printf("Usage: %s k m\nAborting...\n", argv[0]);
        exit(0);
    }

    int k = atoi(argv[1]);
    int m = atoi(argv[2]);

    if(k < 0){
        printf("k must be greater than 0, aborting...\n");
        exit(0);
    }

    printf("total_time,thread_time\n");
    if(m == 0) dispatch_threads_m_const(0, k);
    if(m == 1) dispatch_threads_m_const(10000, k);
    if(m == 2) dispatch_threads_m_prop(1000, k);
    
    return 0;
}
