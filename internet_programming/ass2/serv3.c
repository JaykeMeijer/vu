/*  Assignment 2 - Distributed Programming with Sockets
 *  serv3.c
 *
 * author:   Rik van der Kooij
 * VUnet-ID: rkj800
 * date:     26-09-2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#include "writen_error.h"

#define NUM_CHILD 5

pid_t children[NUM_CHILD];
int shmid,
    *counter,
    sem;

struct sembuf up   = {0, 1, 0};
struct sembuf down = {0, -1, 0};

/* terminate all children, remove the shared memory
 * and remove the semaphore when a SIGINT signal */
void sig_int(int signo)
{
    int i, err_shm = 0, err_sem;

    /* terminate all children */
    for (i = 0; i < NUM_CHILD; i++)
        kill(children[i], SIGTERM);

    while (wait(NULL) > 0);     /* wait for all children */

    /* report error when removing share memory,
     * but continue to remove the semaphore */
    err_shm = shmctl(shmid, IPC_RMID, 0);
    if(err_shm == -1)
        perror("error removing shared memory");

    /* we can exit on semaphore remove error */
    err_sem = semctl(sem, 0, IPC_RMID);
    error_handling(err_sem, "error removing semaphore");

    /* still exit with -1 when removing shared
     * memory failed, but removing semaphore
     * was successful */
    exit(err_shm);
}

/* function which creates a child to handle
 * incoming connections. The parent returns
 * with the process id of the child */
pid_t make_child(int i, int socket_fd, socklen_t len, int sem)
{
    pid_t child_id;
    int child_fd,
        tmp, n;

    struct sockaddr_in addr;

    /* create child process */
    child_id = fork();
    error_handling(child_id, "fork error");

    /* parent returns with child pid */
    if(child_id > 0)
        return child_id;

    /* reset child signal handling for SIGINT */
    signal(SIGINT, SIG_IGN);

    while(1) {
        /* accept incoming connects */
        child_fd = accept(socket_fd, (struct sockaddr*) &addr, &len);
        error_handling(child_fd, "socket accept error");

        /* safely increase the counter by one
         * using a semaphore and temporary 
         * store it in network byte order */
        semop(sem, &down, 1);    
        tmp = htonl((*counter)++);
        semop(sem, &up, 1);    

        /* send the in network byte order counter
         * value to the client */
        n = writen(child_fd, &tmp, sizeof(tmp));
        error_handling(n - 4, "less than 4 bites written");

        /* don't leave the file descriptor open */
        close(child_fd);
    }
}

int main(int argc, const char *argv[])
{
    int socket_fd,
        optval = 1,
        tmp, error, i;

    char input[512];

    struct sockaddr_in addr;
    size_t n;
    socklen_t len;
    pid_t child_id;

    /* kill all children when the parent terminates */
    signal(SIGINT, sig_int);

    /* create a socket */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_handling(socket_fd, "socket creation error");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* set socket options and check for errors */
    error = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,\
                      (const void *)&optval , sizeof(int));
    error_handling(error, "setsocketopt error");

    /* bind protocol address to the socket */
    error = bind(socket_fd, (struct sockaddr *) &addr, sizeof addr); 
    error_handling(error, "bind error");

    /* passive socket needed for server */
    error = listen(socket_fd, 5);
    error_handling(error, "socket listen error");

    /* ignore child exists (kill zombies) */
    signal(SIGCHLD, SIG_IGN);

    /* create semaphore and shared memory */
    sem = semget(IPC_PRIVATE, 1, 0600); 
    semop(sem, &up, 1);    

    shmid = shmget(IPC_PRIVATE, sizeof(int), 0600);
    error_handling(shmid, "shmget error");
    counter = (int *) shmat(shmid, 0, 0); 

    /* counter starts by 1 */
    *counter = 1;

    /* create NUM_CHILD children to handle incoming
     * connections from clients */
    for(i = 0; i < NUM_CHILD; i++) {
        children[i] = make_child(i, socket_fd, len, sem);
    }
    close(socket_fd);

    printf("Server is terminated with 'exit' command, ctr+D or ctr+C\n");

    /* parent waits for 'exit' command, 
     * ctr+D or ctr+C to terminate */
    while(1)
        if(!fgets(input, 512, stdin) || !strcmp(input, "exit\n"))
            /* terminate all children, remove semaphores
             * and shared memory before terminating */
            sig_int(0);
    
    /* this is never reached */
    return 0;
}
