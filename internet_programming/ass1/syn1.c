/*  Assignment 1 - Unix Multiprocessing
 *  syn1.c
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

void display(char *str)
{ 
    char *tmp;
    for(tmp=str; *tmp; tmp++) {
        write(1, tmp, 1);
        usleep(100);
    }
}

int main()
{
    int i, id;
    struct sembuf up   = {0, 1, 0};
    struct sembuf down = {0, -1, 0};

    /* get a semaphore */ 
    id = semget(IPC_PRIVATE, 1, 0600);
    if(id == -1) {
        perror("semget failed");
        exit(-1);  /* exit on semaphore creation error */
    }  
    semop(id, &up, 1);  /* initialize semaphore with 1 */

    if (fork()) {
        for(i=0;i<10;i++) {
            /* lock semaphore (-1), display, and unlock (+1) */
            semop(id, &down, 1);
            display("Hello world\n");
            semop(id, &up, 1);
        }
        wait(NULL);
        semctl(id, 0, IPC_RMID);
    } else {
        for(i=0;i<10;i++) {
            /* lock semaphore (-1), display, and unlock (+1) */
            semop(id, &down, 1);
            display("Bonjour monde\n");
            semop(id, &up, 1);
        }
    } 
    return 0;
}
