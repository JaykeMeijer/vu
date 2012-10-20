#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

/* writen function from Stevens book page 78 */
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
 
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
            if (errno == EINTR)
                nwritten = 0; /* and call write() again */
            else
                return -1;  /* error */
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

/* displays the available command-line options available */
void commands(const char *cmd)
{
    printf("usage: \n");
    printf("%s <hostname> list                      : lists the number of rooms available\n", cmd);
    printf("%s <hostname> book <type> 'guest name'  : book a room. type is a number\n", cmd);
    printf("%s <hostname> guests                    : lists the names of all registered guests\n", cmd);
}

/* checks whether a string represends a 
 * valid number
 *
 * return 0 if not and 1 otherwise
 */
int is_a_number(const char *str)
{
    char *end;
    long value = strtol(str, &end, 10);
    
    if (end == str || *end != '\0' || errno == ERANGE)
        return 0;
    else
        return 1;
}

/* check input for correct commands
 *
 * returns on correct input
 * prints command line and exits on 
 * incorrect input
 */
void check_input(int length, const char *args[])
{
    if(length < 3)
        commands(args[0]);
    else if(!strcmp(args[2], "list"))
        return;
    else if(!strcmp(args[2], "book") && is_a_number(args[3]) && length > 4)
        return;
    else if(!strcmp(args[2], "guests"))
        return;
    else 
        commands(args[0]);
    exit(-1);
}

int main(int argc, const char *argv[])
{
    int socket_fd, i;
    struct addrinfo *res, *res0, hints;
    ssize_t nread, nwriten;
    char buf[50] = "", input[300] = "";
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    /* print commands on incorrect input */
    check_input(argc, argv);
    
    /* concate all arguments together in order
     * to send them as one message over the socket */
    for(i = 2; i < argc; i++) {
        if(i != 2)
            strncat(input, " ", 2); 
        strncat(input, argv[i], strlen(argv[i]));
    }

    getaddrinfo(argv[1], "1234", &hints, &res0);
    
    /* find a socket which we can connect to */
    for(res = res0; res; res = res0->ai_next) {
        socket_fd = socket(res->ai_family, res0->ai_socktype, res0->ai_protocol);

        /* try to connect with the gateway */
        if(connect(socket_fd, res->ai_addr, res0->ai_addrlen) >= 0)
            break;
    }

    if(!res) {
        perror("No connection found");
        exit(-1);
    }

    /* send the command to the gateway */
    nwriten = writen(socket_fd, input, strlen(input));

    /* retrieve and print answer from the gateway */
    while((nread = read(socket_fd, (void *)buf, 49))) {
        buf[nread] = '\0';
        printf("%s", buf);
    }
    printf("\n");
    
    return 0;
}
