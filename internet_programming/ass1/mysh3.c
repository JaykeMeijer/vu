#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int process_input(char* input, char** args, char** args2)
{
    int i = 0, j = 0;
    char *token = strtok(input, " ");

    do {
        if(!strcmp(token, "|"))
            j = i;

        if(!j)
            args[i] = token;
        if(j && j != i)
            args2[i-j-1] = token;
        i++;
    } while(token = strtok(NULL, " "));
    args[j+1] = NULL;
    args2[i] == NULL;
    
    return j;
}

int main(int argc, const char* argv[])
{
    char input[512],
         *args[256],
         *args2[256];

    pid_t child_id,
          gchild_id;

    int fd[2],
        i,
        j;

    while(1) {
        fputs("$ ", stdout);
        fflush(stdout);
        if(!fgets(input, 512, stdin) || !strcmp(input, "exit\n"))
            break;
        
        input[strlen(input) - 1] = '\0';
        j = process_input(input, args, args2);

        child_id = fork();

        if(!child_id) {
            if(j) {
                if(pipe(fd) < 0)
                    exit(1);
                gchild_id = fork();

                if(gchild_id == 0) {  // i write
                    close(fd[0]);
                    dup2(fd[1], 1);
                    execvp(args[0], args);
                }
                else { // i read
                    close(fd[1]);
                    dup2(fd[0], 0);
                    execvp(args2[0], args2);
                }
            }
            else {
                execvp(args[0], args);
            }
            printf("[%d] -bash: %s: command not found\n", child_id, input);
            break;
        }
        else if(child_id > 0)
            waitpid(child_id, 0, 0);
    }
    return 0;
}
