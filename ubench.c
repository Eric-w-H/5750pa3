#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BILLION 1000000000L


typedef struct {
    struct timespec start, end;
    int* a;
} args;


void ubench(void* parm)
{
    args* arg = (args*) parm;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
}

int main(int argc, char** argv)
{
    // Parse command line
    if(argc != 3) {
        printf("Usage: %s n m\nAborting...\n", argc[0]);
        exit(0);
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    if(n < 0){
        printf("n must be greater than 0, aborting..."\n);
        exit(0);
    }
    
    if(m < 0){
        printf("m must be greater than 0, aborting..."\n);
        exit(0);
    }


    // Setup


    // Execution
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    clock_gettime(CLOCK_MONOTONIC, &end);


    // Output
    time = BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed: %lf seconds\n", time);
    printf("");


}