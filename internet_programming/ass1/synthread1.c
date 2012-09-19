#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mutexsum;

void display(char *str)
{ 
    char *tmp;
    for(tmp = str; *tmp; tmp++) {
        write(1, tmp, 1);
        usleep(100);
    }
}

void *hello(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutexsum);
        display("Hello world\n");
        pthread_mutex_unlock(&mutexsum);
        //usleep(2000);
    }
    return;
}

void *bonjour(void *param) {
    int i;

    for(i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutexsum);
        display("Bonjour monde\n");
        pthread_mutex_unlock(&mutexsum);
    }
    return;
}

int main()
{
    int i,
        j;

    pthread_t hel_id,
              bon_id;

    pthread_mutex_init(&mutexsum, NULL);

    i = pthread_create(&hel_id, NULL, hello, NULL);
    j = pthread_create(&bon_id, NULL, bonjour, NULL);
    
    if(i == -1 || j == -1) {
        perror("Threads creation error");
        exit(-1);
    }

    pthread_join(hel_id, NULL);
    pthread_join(bon_id, NULL);
    pthread_mutex_destroy(&mutexsum);

    return 0;
}
