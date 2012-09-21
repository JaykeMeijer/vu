#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void print(char** args)
{
    int i = 0;
    printf("-bash: ");
        printf("%s ", args[i++]);
        printf("%d ", args != 0);
    printf(": command not found\n");
    fflush(stdout);

}

void get_arguments(char* input, char** args)
{
    int i = 0;
    char *token = strtok(input, " ");

    do {
        args[i++] = token;
    } while(token = strtok(NULL, " "));
    args[i] = NULL;
}

int process_input(char* input, char** args, char** args2)
{
    int i = 0, j = 0;
    char *token = strtok(input, "|");

    do {
        if(i == 0)
            args[255] = token; 
        if(i == 1)
            args2[255] = token;
        i++;
    } while(token = strtok(NULL, "|"));

    get_arguments(args[255], args);
    get_arguments(args2[255], args2);

    return i - 1;
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
        piped_commands;

    /* run shell forever */
    while(1) {
        fputs("$ ", stdout);
        fflush(stdout);

        /* get user input */
        if(!fgets(input, 512, stdin) || !strcmp(input, "exit\n"))
            break; /* exit on EOF or exit command */
        
        /* remove newline at the end */ 
        input[strlen(input) - 1] = '\0';

        /* get command and arguments from input */
        piped_commands = process_input(input, args, args2);

        child_id = fork();

        if(!child_id) { /* child executes given commands */
            if(piped_commands) { 
                if(pipe(fd) < 0) {
                    perror("Pipe creation error");
                    exit(1);  /* exit on pipe error */
                }
                gchild_id = fork();

                if(gchild_id == 0) {  /* grand child executes first command */
                    close(fd[0]);  /* close reading pipe */
                    dup2(fd[1], 1);  /* map stdout to pipe */
                    execvp(args[0], args);
                    dup2(1, 1);
                    print(args);  /* print on exec error */
                }
                else {  /* child executes second command */
                    close(fd[1]);  /* close writing pipe */
                    dup2(fd[0], 0);  /* map stdin to pipe */
                    execvp(args2[0], args2);
                    print(args2);  /* print on exec error */
                }
            }
            else {
                execvp(args[0], args);  /* run the only command given */
                printf("-bash: %s: command not found\n", input);
            }
            break;  /* exit all childs after exec errors */
        }
        else if(child_id > 0)
            waitpid(child_id, 0, 0);  /* wait for the child */
    }
    return 0;
}
