/*  Assignment 1 - Unix Multiprocessing
 *  syn2.c
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

void display(char *str) {
    char *tmp;
    for(tmp=str; *tmp; tmp++) {
        write(1, tmp, 1);
        usleep(100);
    }
}

int main() {
    int i, sem_ab, sem_cd;
    struct sembuf up   = {0, 1, 0};
    struct sembuf down = {0, -1, 0};

    pid_t child_id;

    /* get two shemaphores */
    sem_ab = semget(IPC_PRIVATE, 1, 0600);
    sem_cd = semget(IPC_PRIVATE, 1, 0600);
    
    if(sem_ab == -1 || sem_cd == -1) {
        perror("semget failed");
        exit(-1);  /* exit on semaphore creation error */
    }
    semop(sem_ab, &up, 1);  /* initialize semaphore ab with 1 */
    child_id = fork();

    if(child_id) {
        for(i = 0; i < 10; i++) {
            semop(sem_ab, &down, 1);  /* down semaphore ab */
            display("ab");
            semop(sem_cd, &up, 1);    /* up semaphore cd */
        }
    } else {
        for(i = 0; i < 10; i++) {
            semop(sem_cd, &down, 1);  /* down semaphore cd */
            display("cd\n");
            semop(sem_ab, &up, 1);    /* up sempahore ab */
        }
    }

    /* wait for child to end */
    waitpid(child_id, 0, 0);  

    /* remove semaphores */
    semctl(sem_ab, 0, IPC_RMID);
    semctl(sem_cd, 0, IPC_RMID);

    return 0;
}
