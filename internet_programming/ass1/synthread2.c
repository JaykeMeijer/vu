/*  Assignment 1 - Unix Multiprocessing
 *  synthread2.c
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
#include <sys/sem.h>

pthread_cond_t cond_ab;
pthread_cond_t cond_cd;

pthread_mutex_t cond_mutex;

int predicate = 0;

void display(char *str) {
    char *tmp;
    for(tmp=str; *tmp; tmp++) {
        write(1, tmp, 1);
        usleep(100);
    }
}

void *ab(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        /* lock mutex, display, signal cd thread and wait for signal of cd */
        pthread_mutex_lock(&cond_mutex);
        predicate = 1;
        display(param);
        pthread_cond_signal(&cond_cd);
        while(predicate)
            pthread_cond_wait(&cond_ab, &cond_mutex);
        pthread_mutex_unlock(&cond_mutex);
    }
    return;
}

void *cd(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        /* lock mutex, wait for signal of ab, display and signal ab */
        pthread_mutex_lock(&cond_mutex);
        while(!predicate)
            pthread_cond_wait(&cond_cd, &cond_mutex);
        display(param);
        predicate = 0;
        pthread_cond_signal(&cond_ab);
        pthread_mutex_unlock(&cond_mutex);
    }
    return;
}

int main() {
    int hel,
        bon;
    
    pthread_t hel_id,
              bon_id;

    pthread_cond_init(&cond_ab, NULL);
    pthread_cond_init(&cond_cd, NULL);
    pthread_mutex_init(&cond_mutex, NULL);

    /* create threads with the corresponding strings */
    hel = pthread_create(&hel_id, NULL, ab, "ab");
    bon = pthread_create(&bon_id, NULL, cd, "cd\n");

    /* exit on thread error */
    if(hel == -1 || bon == -1) {
        perror("Threads creation error");
        exit(-1);
    }

    /* join threads and destroy mutex and condition variables before exiting */ 
    pthread_join(hel_id, NULL);
    pthread_join(bon_id, NULL);

    pthread_cond_destroy(&cond_ab);
    pthread_cond_destroy(&cond_cd);

    return 0;
}
