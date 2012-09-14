#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int process_input(char* input, char** args)
{
    int i = 0;
    char *token = strtok(input, " ");

    do {
        args[i++] = token;
    } while(token = strtok(NULL, " "));
    args[i] = NULL;
    
    return i;
}

int main(int argc, const char* argv[])
{
    char input[512],
         *args[256];

    pid_t child_id;

    int i;

    while(1)
    {
        fputs("$ ", stdout);
        fflush(stdout);
        if(!fgets(input, 512, stdin) || !strcmp(input, "exit\n"))
            break;
        
        input[strlen(input) - 1] = '\0';
        process_input(input, args);

        child_id = fork();

        if(!child_id) {
            execvp(args[0], args);
            printf("-bash: %s: command not found\n", input);
            break;
        }
        else if(child_id > 0)
            waitpid(child_id, 0, 0);
    }
    return 0;
}
