#include <stdio.h>
#include <stdlib.h>
#include "add.h"

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
    CLIENT *cl;
    param in;
    add_out *out;
    
    if(argc < 2)
        show_options(argv[0]);

    cl = clnt_create(argv[1], PAPER_SERVER, PAPER_VERSION, "tcp");
    in.name = "Jan jansen"; 
    in.title = "Test paper"; 
    in.next = 0;

    out = add_1(&in, cl);
    printf("out.arg: %d\n", (int)*out);
    return 0;
}
