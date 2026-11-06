/*
 * Chapter 9.6 - Threads with POSIX pthreads
 * Build: gcc -pthread 06_threads.c -o threads
 */
#include <pthread.h>
#include <stdio.h>

#define THREADS 4
#define N 40000000L

typedef struct {
    long start, end;      /* this worker's slice */
    double partial;       /* its result */
} Job;

static void *sum_slice(void *arg)                /* a thread runs a function taking void * */
{
    Job *job = arg;
    double s = 0;
    for (long i = job->start; i < job->end; i++)
        s += 1.0 / (1.0 + (double)i * i);         /* any slow maths */
    job->partial = s;
    return NULL;
}

static long counter = 0;
static pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

static void *count_up(void *arg)
{
    (void)arg;
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&counter_lock);       /* one thread at a time in here */
        counter++;
        pthread_mutex_unlock(&counter_lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t workers[THREADS];
    Job jobs[THREADS];
    for (int t = 0; t < THREADS; t++) {
        jobs[t].start = t * (N / THREADS);
        jobs[t].end = (t + 1) * (N / THREADS);
        pthread_create(&workers[t], NULL, sum_slice, &jobs[t]);
    }
    double total = 0;
    for (int t = 0; t < THREADS; t++) {
        pthread_join(workers[t], NULL);          /* wait for each to finish */
        total += jobs[t].partial;
    }
    printf("sum split over %d threads = %.6f\n", THREADS, total);

    pthread_t a, b;
    pthread_create(&a, NULL, count_up, NULL);
    pthread_create(&b, NULL, count_up, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("two threads x 100000 increments = %ld (always 200000 thanks to the mutex)\n", counter);
    return 0;
}
