#include <pthread.h>

typedef struct Thread {
    pthread_t * pid;
    struct Thread * next;    
} Thread_t;