/* error exit function 
 * prints error string parameter given 
 * and perror before exiting on a 
 * negative integer */
void error_handling(int error, char *str);

/* writen function from Stevens book page 78 */
ssize_t writen(int fd, const void *vptr, size_t n);
