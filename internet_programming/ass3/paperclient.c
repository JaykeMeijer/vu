#include <stdio.h>
#include <stdlib.h>
#include "headers/writen_error.h"

void show_options(const char *str)
{
    printf("usage: %s <hostname> <action> <arguments>\n", str);
    printf("actions:\n"); 
    printf("store article:    add <Author Name> <Paper Title> <file>\n");
    printf("remove article:   remove <number>\n");
    printf("list articles:    list\n");
    printf("display info of an article:  info <number>\n");
    printf("fetch an article: fetch <number>\n\n");
    printf("example add:    %s localhost remove 4\n", str);
    exit(-1);
}


int main(int argc, const char *argv[])
{
    int i;

    if(argc < 2)
        show_options(argv[0]);
    else {


    }
    return 0;
}
