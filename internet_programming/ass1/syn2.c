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

    sem_ab = semget(IPC_PRIVATE, 1, 0600);
    sem_cd = semget(IPC_PRIVATE, 1, 0600);
    
    if(sem_ab == -1 || sem_cd == -1) {
        perror("semget failed");
        exit(-1);
    }
    semop(sem_ab, &up, 1);

    if(fork()) {
        for(i = 0; i < 10; i++) {
            semop(sem_ab, &down, 1);
            display("ab");
            semop(sem_cd, &up, 1);
        }
        wait(NULL);
        semctl(sem_ab, 0, IPC_RMID);
        semctl(sem_cd, 0, IPC_RMID);
    }
    else {
        for(i = 0; i < 10; i++) {
            semop(sem_cd, &down, 1);
            display("cd\n");
            semop(sem_ab, &up, 1);
        }
    }
    return 0;
}
