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

#define NUM_CHILD 5

pid_t children[NUM_CHILD];
int shmid,
    *shd_int,
    sem;

struct sembuf up   = {0, 1, 0};
struct sembuf down = {0, -1, 0};

void error_handling(int error, char *str);

void sig_int(int signo)
{
    int i, error;

    /* terminate all children */
    for (i = 0; i < NUM_CHILD; i++)
        kill(children[i], SIGTERM);

    while (wait(NULL) > 0);     /* wait for all children */

    i = shmctl(shmid, IPC_RMID, 0);
    error = semctl(sem, 0, IPC_RMID);
    if(error < -1)
        printf("ERROR %d\n", error);

    printf("i: %d  error: %d\n", i, error);

    exit(0);
}

/* error exit function */
void error_handling(int error, char *str)
{
    if(error < 0) {
        perror(str);
        exit(-1);
    }
}

/* writen function from slides */
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (errno == EINTR)
                nwritten = 0; /* and call write() again */
            else 
                return -1;  /* error */
        } 
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

pid_t make_child(int i, int socket_fd, socklen_t len, int sem)
{
    pid_t child_id;
    int child_fd,
        tmp, n;

    struct sockaddr_in addr;

    /* create child process */
    child_id = fork();

    /* parent returns with child pid */
    if(child_id > 0)
        return child_id;

    /* child accepts incomming connects and sends count */
    //printf("Child %d reporting for duty.\n", i);

    while(1) {
        /* accept incomming connects */
        child_fd = accept(socket_fd, (struct sockaddr*) &addr, &len);
        error_handling(child_fd, "socket accept error");

        /* send counter value to client */
        tmp = htonl(*shd_int);
        n = writen(child_fd, &tmp, sizeof(*shd_int));
        error_handling(n - 4, "less than 4 bites written");

        /* safely increase the counter by one */
        semop(sem, &down, 1);    
        *shd_int += 1;
        semop(sem, &up, 1);    

        close(child_fd);
    }
}


int main(int argc, const char *argv[])
{
    int socket_fd,
        counter = 0, optval = 1,
        tmp, error, i;

    struct sockaddr_in addr;
    size_t n;
    socklen_t len;
    pid_t child_id;

    /* kill all children on parent termination */
    signal(SIGINT, sig_int);
    printf("DIT IS GEDAAN\n");

    /* create socket */
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
    shd_int = (int *) shmat(shmid, 0, 0); 

    *shd_int = 1;

    for(i = 0; i < NUM_CHILD; i++) {
        children[i] = make_child(i, socket_fd, len, sem);
    }
    close(socket_fd);

    /* just wait forever */
    while(1);

    sig_int(0);
    return 0;
}
