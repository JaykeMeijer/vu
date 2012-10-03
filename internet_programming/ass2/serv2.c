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

#include "writen_error.h"

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

    /* set socket level options */
    error = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,\
                      (const void *)&optval , sizeof(int));
    error_handling(error, "setsocketopt error");

    /* bind protocol address to the socket */
    error = bind(socket_fd, (struct sockaddr *) &addr, sizeof addr); 
    error_handling(error, "bind error");

    /* passive socket needed for server */
    error = listen(socket_fd, 5);
    error_handling(error, "socket listen error");

    /* ignore child exist signals (kill zombies) */
    signal(SIGCHLD, SIG_IGN);

    while(1) {
        /* accept incomming connects from clients */
        child_fd = accept(socket_fd, (struct sockaddr*) &addr, &len);
        error_handling(child_fd, "socket accept error");

        /* increasing the counter before forking solves
         * all concurrency problems */
        counter++;

        /* fork a child to handle the request */
        child_id = fork();
        error_handling(child_id, "fork error");

        if(!child_id) {
            /* send counter value to client
             * but first convert it from host 
             * byte order to network byte order */
            tmp = htonl(counter);
            n = writen(child_fd, &tmp, sizeof(counter));
            error_handling(n - 4, "less than 4 bites written"); 

            /* close the file descriptor and terminate */ 
            close(child_fd);
            break;
        }
        /* parent should also close the file descriptor */
        close(child_fd);
    }
    return 0;
}
