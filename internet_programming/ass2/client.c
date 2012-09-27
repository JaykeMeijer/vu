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

/* error exit function */
void error_handling(int error, char *str)
{
    if(error < 0) {
        perror(str);
        exit(-1);
    }
}

int main(int argc, const char *argv[])
{
    int socket_fd, error, 
        i = 0, tmp = 0;

    struct addrinfo *res0, *res, hints;
    
    void *buf = malloc(sizeof(char) * 4);
    ssize_t nread;

    if (argc < 2) {
        printf("usage: %s <host_name>\n", argv[0]);
        exit(-1);
    }
   
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* get a list of address */
    error = getaddrinfo(argv[1], "1234", &hints, &res);
    error_handling(error, "getaddrinfo error");

    /* find a socket which we can connect to */
    for(res0 = res; res0; res0 = res->ai_next) {
        socket_fd = socket(res0->ai_family, res->ai_socktype, res->ai_protocol);

        if(connect(socket_fd, res0->ai_addr, res->ai_addrlen) >= 0)
            break;
    }

    /* retrieve the counter value */
    nread = read(socket_fd, &tmp, 4);
    error_handling(nread - 4, "read less than 4 bytes");

    printf("%d\n", ntohl(tmp));
    
    return 0;
}
