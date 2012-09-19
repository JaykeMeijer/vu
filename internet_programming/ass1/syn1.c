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

    id = semget(IPC_PRIVATE, 1, 0600);
    if(id == -1) {
        perror("semget failed");
        exit(-1);
    }  
    semop(id, &up, 1);

    if (fork()) {
        for(i=0;i<10;i++) {
            semop(id, &down, 1);
            display("Hello world\n");
            semop(id, &up, 1);
        }
        wait(NULL);
        semctl(id, 0, IPC_RMID);
    } else {
        for(i=0;i<10;i++) {
            semop(id, &down, 1);
            display("Bonjour monde\n");
    semop(id, &up, 1);
        }
    } 
    return 0;
}
