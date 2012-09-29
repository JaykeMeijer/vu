/*  Assignment 2 - Distributed Programming with Sockets
 *  serv2.c
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

int main(int argc, const char *argv[])
{
    int socket_fd, child_fd,
        counter = 0, optval = 1,
        tmp, error;

    struct sockaddr_in addr;
    size_t n;
    socklen_t len;
    pid_t child_id;

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

    while(1) {
        /* accept incomming connects */
        child_fd = accept(socket_fd, (struct sockaddr*) &addr, &len);
        error_handling(child_fd, "socket accept error");

        counter++;
        child_id = fork();
        error_handling(child_id, "fork error");

        if(!child_id) {
            /* send counter value to client */
            tmp = htonl(counter);
            n = writen(child_fd, &tmp, sizeof(counter));
            error_handling(n - 4, "less than 4 bites written");
            
            close(child_fd);
            break;
        }
        close(child_fd);
    }
    return 0;
}
