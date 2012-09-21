/*  Assignment 1 - Unix Multiprocessing
 *  mysh2.c
 *
 * author:   Rik van der Kooij
 * VUnet-ID: rkj800
 * date:     20-09-2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* get the arguments from the input */
void process_input(char* input, char** args)
{
    int i = 0;
    char *token = strtok(input, " ");

    do {
        args[i++] = token;
    } while(token = strtok(NULL, " "));
    args[i] = NULL;
}

int main(int argc, const char* argv[])
{
    char input[512],
         *args[256];

    pid_t child_id;

    int i;

    /* run the shell forever */
    while(1)
    {
        fputs("$ ", stdout);
        fflush(stdout); 

        /* get user input */
        if(!fgets(input, 512, stdin) || !strcmp(input, "exit\n"))
            break;  /* exit on EOF or exit command */
        
        /* remove newline at the end */
        input[strlen(input) - 1] = '\0';

        /* create argument array */
        process_input(input, args);

        child_id = fork();

        if(!child_id) {  /* child executes given command */
            execvp(args[0], args);
            printf("-bash: %s: command not found\n", input);
            break;  /* exit child on command error */
        }
        else if(child_id > 0)  /* wait for child to end */
            waitpid(child_id, 0, 0);
    }
    return 0;
}
