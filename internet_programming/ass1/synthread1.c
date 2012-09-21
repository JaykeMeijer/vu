/*  Assignment 1 - Unix Multiprocessing
 *  synthread1.c
 *
 * author:   Rik van der Kooij
 * VUnet-ID: rkj800
 * date:     20-09-2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

/* mutex for mutual exclusion of display method */
pthread_mutex_t mutexsum;

void display(char *str)
{ 
    char *tmp;
    for(tmp = str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}

/* run method for threads */
void *run(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        /* lock mutex to create mutual exclusion for display method */
        pthread_mutex_lock(&mutexsum);
        display(param);
        pthread_mutex_unlock(&mutexsum);
    }
    return;
}

int main()
{
    int hel,
        bon;

    pthread_t hel_id,
              bon_id;

    pthread_mutex_init(&mutexsum, NULL);

    /* create threads with the corresponding strings */
    hel = pthread_create(&hel_id, NULL, run, "Hello world\n");
    bon = pthread_create(&bon_id, NULL, run, "Bonjour monde\n");
    
    /* exit on thread error */ 
    if(hel == -1 || bon == -1) {
        perror("Threads creation error");
        exit(-1);
    }
    
    /* join threads and destroy mutex before exiting */
    pthread_join(hel_id, NULL);
    pthread_join(bon_id, NULL);
    pthread_mutex_destroy(&mutexsum);

    return 0;
}
