/*  Assignment 1 - Distributed Programming with Sockets
 *  client.c
 *
 * author:   Rik van der Kooij
 * VUnet-ID: rkj800
 * date:     26-09-2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "writen_error.h"

int main(int argc, const char *argv[])
{
    int socket_fd, error = 0, 
        counter = 0;

    struct addrinfo *res0, *res, hints;
    
    void *buf = malloc(sizeof(char) * 4);
    ssize_t nread;

    /* exit on no input */
    if (argc < 2) {
        printf("usage: %s <host_name>\n", argv[0]);
        exit(-1);
    }
   
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* get a list of possible addresses */
    error = getaddrinfo(argv[1], "1234", &hints, &res);
    error_handling((error == 0) - 1, "getaddrinfo error");

    /* find a socket which we can connect to */
    for(res0 = res; res0; res0 = res->ai_next) {
        socket_fd = socket(res0->ai_family, res->ai_socktype, res->ai_protocol);
        error_handling(socket_fd, "socket creation error");

        if(connect(socket_fd, res0->ai_addr, res->ai_addrlen) >= 0)
            break;
    }

    /* no connection found */
    if(!res0) {
        perror("no connection found");
        exit(-1);
    }

    /* retrieve the counter value */
    nread = read(socket_fd, &counter, 4);
    error_handling(nread - 4, "read less than 4 bytes");

    /* print the counter value */
    printf("%d\n", ntohl(counter));
    
    return 0;
}
