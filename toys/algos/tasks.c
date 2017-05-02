#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t) 
{
    int i;
    long id = (long) t;
    printf("Starting inc_count(): thread %lx\n", id);
    for (i=0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;
        /* 
            Check the value of count and signal waiting thread when condition is
            reached.  Note that this occurs while mutex is locked. 
        */
        if (count == COUNT_LIMIT) {
            printf("inc_count(): thread %lx, count = %d  Threshold reached. ", id, count);
            pthread_cond_signal(&count_threshold_cv);
            printf("Just sent signal.\n");
        }
        printf("inc_count(): thread %lx, count = %d, unlocking mutex\n", id, count);
        pthread_mutex_unlock(&count_mutex);
        /* Do some work so threads can alternate on mutex lock */
        sleep(1);
    }
    pthread_exit(NULL);
}

void *watch_count(void *t) 
{
    long id = (long) t;
    printf("Starting watch_count(): thread %lx\n", id);
    /*
        Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
        will automatically and atomically unlock mutex while it waits. 
        Also, note that if COUNT_LIMIT is reached before this routine is run by
        the waiting thread, the loop will be skipped to prevent pthread_cond_wait
        from never returning.
    */
    pthread_mutex_lock(&count_mutex);
    while (count < COUNT_LIMIT) {
        printf("watch_count(): thread %lx Count= %d. Going into wait...\n", id, count);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        printf("watch_count(): thread %lx Condition signal received. Count= %d\n", id,count);
    }
    printf("watch_count(): thread %lx Updating the value of count...\n", id);
    count += 125;
    printf("watch_count(): thread %lx count now = %d.\n", id, count);
    printf("watch_count(): thread %lx Unlocking mutex.\n", id);
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}


#ifndef _NO_MAIN_
int main()
#else
int test_tasks()
#endif
{
    int i, rc; 
    long t1=0x42, t2=0x17, t3=0x53;
    pthread_t threads[3];
    pthread_attr_t attr;

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init (&count_threshold_cv, NULL);

    /* For portability, explicitly create threads in a joinable state */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, watch_count, (void *)t1);
    pthread_create(&threads[1], &attr, inc_count, (void *)t2);
    pthread_create(&threads[2], &attr, inc_count, (void *)t3);

    /* Wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf ("Main(): Waited and joined with %d threads. Final value of count = %d. Done.\n", NUM_THREADS, count);

    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit (NULL);

    return 0;
}
