/*  Assignment 2 - Distributed Programming with Sockets
 *  talk.c
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
#include <string.h>


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

void wait_for_input(int socket_fd)
{
    int n;
    char input[512];


    while(1) {
        if(!fgets(input, 512, stdin))
            break;
        n = writen(socket_fd, &input, sizeof input);
        error_handling(n - (sizeof input), "not full input written");
    }
    close(socket_fd);

    return;
}

void wait_for_message(int socket_fd)
{
    int n; 
    char message[512];
    for(n = 0; n < 512; n++)
        message[n] = '\0';


    while(1) {
        n = read(socket_fd, message, 511);

        printf("%s", message);
    }
    return;
}

void start_as_server()
{
    int socket_fd, child_fd,
        error, optval = 1;

    struct sockaddr_in addr;
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

    child_fd = accept(socket_fd, (struct sockaddr*) &addr, &len);
    error_handling(child_fd, "socket accept error");

    child_id = fork();
    error_handling(child_id, "fork error");

    if(child_id)
        wait_for_input(child_fd);
    else
        wait_for_message(child_fd);
    return;
}

void start_as_client(const char *name)
{
    int socket_fd,
        error;

    pid_t child_id;
    struct addrinfo *res0, *res, hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* get a list of address */
    error = getaddrinfo(name, "1234", &hints, &res);
    error_handling(error, "getaddrinfo error");

    /* find a socket which we can connect to */
    for(res0 = res; res0; res0 = res->ai_next) {
        socket_fd = socket(res0->ai_family, res->ai_socktype, res->ai_protocol);

        if(connect(socket_fd, res0->ai_addr, res->ai_addrlen) >= 0)
            break;
    }

    child_id = fork();
    error_handling(child_id, "fork error");

    if(child_id)
        wait_for_input(socket_fd);
    else
        wait_for_message(socket_fd);
    return;
}

int main(int argc, const char *argv[])
{
    if(argc < 2)
        start_as_server();
    else
        start_as_client(argv[1]);

    return 0;
}
