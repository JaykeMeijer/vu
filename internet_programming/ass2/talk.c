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

#include "writen_error.h"

/* function waits for input from user
 * and sends it over the socket to
 * the other talk user
 * 
 * an exit command or ctr+D terminates
 * the talk program/processes
 */
void wait_for_input(int socket_fd)
{
    int n;
    char input[512];

    while(1) {
        /* get input from user. terminate on
         * a 'exit' command or a ctr+D */
        if(!fgets(input, 512, stdin) || !strcmp(input, "quit()\n")) {
            /* since the connection is closed we 
             * should notify/shutdown the other 
             * talk user by sending a \0 char */
            n = writen(socket_fd, '\0', sizeof input);
            break;
        }

        /* send the input over the socket to the other user */
        n = writen(socket_fd, &input, sizeof input);
        error_handling(n - (sizeof input), "not full input written");
    }
    close(socket_fd);

    return;
}

/* function waits for for a message
 * from the other talk user and
 * writes it to stdout
 * 
 * terminate when no bytes are read
 * from the socket */
void wait_for_message(int socket_fd)
{
    int n; 
    char message[512];

    /* initialize the message string as empty */
    memset(message, 0, 512);
    while(1) {
        n = read(socket_fd, message, 511);

        /* terminate when we dont read
         * anything/connection closes */
        if(n == 0)
            /* we can just exit as a child since
             * the parent process terminates on
             * oru SIGCHLD singal */
            exit(-2);
        
        printf("%s", message);
    }
    return;
}

/* create two processes: 
 * one to handle input from
 * the user which it sends 
 * over the connected socket_fd
 *
 * and one to wait for messages
 * from the other user to print
 * on the screen
 */
pid_t start_input_output(int socket_fd) 
{
    pid_t child_id;

    child_id = fork();
    error_handling(child_id, "fork error");
    
    /* the parent handles input and sets
     * SIGCHLD signal to terminate itself
     * when the child terminates*/
    if(child_id) {
        signal(SIGCHLD, exit);
        wait_for_input(socket_fd);
    } else
        /* the child waits for messages from
         * the other talk user */
        wait_for_message(socket_fd);

    return child_id;
}

/* creates a socket as a server and
 * waits for an incomming talk client.
 *
 * the return value is used for the
 * parent process to terminate its
 * child */
pid_t start_as_server()
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

    /* accept connecting client */
    child_fd = accept(socket_fd, (struct sockaddr*) &addr, &len);
    error_handling(child_fd, "socket accept error");

    /* and messages from other talk user */
    return start_input_output(child_fd);
}

/* creates a socket as a client and
 * connect to a talk server
 *
 * the return value is used for the
 * parent process to terminate its
 * child */
pid_t start_as_client(const char *name)
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

    /* exit the client when no connection is found */
    if(!res0) {
        perror("No connection found");
        exit(-1);
    }

    /* connection is established start
     * two processes to handle user input
     * and messages from other talk user */
    return start_input_output(socket_fd);
}

int main(int argc, const char *argv[])
{
    pid_t child_id;

    /* start as server when no 
     * input is given */
    if(argc < 2)
        child_id = start_as_server();
    else
        /* input is server address,
         * start as client */
        child_id = start_as_client(argv[1]);

    /* upon returning from start_as_server
     * or start_as_client we want to terminate.
     * parents should terminate the child
     *
     * children should not be able to reach
     * this */
    if(child_id) {  /* safety check */
        kill(child_id, SIGTERM);
        waitpid(child_id, 0, 0);
    }
    return 0;
}
