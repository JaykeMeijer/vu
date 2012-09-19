#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>

pthread_mutex_t mutex_ab,
                mutex_cd;

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

void *hello(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        pthread_mutex_lock(&cond_mutex);
        predicate = 1;
        display("ab");
        pthread_cond_signal(&cond_cd);
        while(predicate)
            pthread_cond_wait(&cond_ab, &cond_mutex);
        pthread_mutex_unlock(&cond_mutex);
    }
    return;
}

void *bonjour(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        pthread_mutex_lock(&cond_mutex);
        while(!predicate)
            pthread_cond_wait(&cond_cd, &cond_mutex);
        display("cd\n");
        predicate = 0;
        pthread_cond_signal(&cond_ab);
        pthread_mutex_unlock(&cond_mutex);
    }
    return;
}

int main() {
    int i,
        j;
    
    pthread_t hel_id,
              bon_id;

    pthread_cond_init(&cond_ab, NULL);
    pthread_cond_init(&cond_cd, NULL);
    pthread_mutex_init(&cond_mutex, NULL);


    i = pthread_create(&hel_id, NULL, hello, NULL);
    j = pthread_create(&bon_id, NULL, bonjour, NULL);

    if(i == -1 || j == -1) {
        perror("Threads creaton error");
        exit(-1);
    }

    
    pthread_join(hel_id, NULL);
    pthread_join(bon_id, NULL);
    pthread_mutex_destroy(&mutex_ab);
    pthread_mutex_destroy(&mutex_cd);

    return 0;
}
