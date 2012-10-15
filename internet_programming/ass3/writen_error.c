#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <errno.h>

/* error exit function 
 * prints error string parameter given 
 * and perror before exiting on a 
 * negative integer */
void error_handling(int error, char *str)
{
    if(error < 0) {
        perror(str);
        exit(-1);
    }
}

/* writen function from Stevens book page 78 */
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