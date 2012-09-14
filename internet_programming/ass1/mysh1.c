#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    char input[512],
         *args[2];

    pid_t child_id;

    while(1)
    {
        fputs("$ ", stdout);
        fflush(stdout);
        if(!fgets(input, 512, stdin) || !strcmp(input, "exit\n"))
            break;

        input[strlen(input) - 1] = '\0';

        args[0] = input;
        args[1] = (char *)NULL;

        child_id = fork();

        if(!child_id) {
            execvp(input, args);
            printf("-bash: %s: command not found\n", input);
            break;
        }
        else if(child_id > 0)
            waitpid(child_id, 0, 0);
    }
    return 0;
}
