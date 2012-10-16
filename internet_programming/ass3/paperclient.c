#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "add.h"

void show_options(const char *str)
{
    printf("usage: %s <hostname> <action> <arguments>\n", str);
    printf("actions:\n"); 
    printf("store article:    add <Author Name> <Paper Title> <file>\n");
    printf("remove article:   remove <number>\n");
    printf("list articles:    list\n");
    printf("article info:     info <number>\n");
    printf("fetch an article: fetch <number>\n\n");
    printf("example remove:   %s localhost remove 4\n", str);
    exit(-1);
}

void add(const char *arg[])
{
    FILE *f;
    long size;

    printf("adding paper\n");

    f = fopen(arg[2], "rb");
    if(f == NULL) {
        printf("File %s not found", arg[2]);
        exit(-1);
    }
    /* get size of file */
    if(!fseek(f, 0, SEEK_END)) {
        perror("fseek failed");
        exit(-1);
    }
    size = ftell(f);
    if(size < 0) {
        perror("ftell failed");
        exit(-1);
    }
    rewind(f);
    fclose(f);

    printf("%ld", size);

    return;
}
void remove_paper() { return; }
void list() { return; }
void info() { return; }
void fetch() { return; }


int main(int argc, const char *argv[])
{
    CLIENT *cl;
    //param in;
    //add_out *out;
    
    if(argc < 3)
        show_options(argv[0]);

    cl = clnt_create(argv[1], PAPER_SERVER, PAPER_VERSION, "tcp");
    if(cl == NULL) {
        perror("No RPC server found");
        exit(-1);
    }

    if(!strcmp(argv[2], "add") && argc >= 6)
        add(argv+3);
    else if(!strcmp(argv[2], "remove") && argc >= 4)
        remove_paper();
    else if(!strcmp(argv[2], "list"))
        list();
    else if(!strcmp(argv[2], "info"))
        info();
    else if(!strcmp(argv[2], "fetch") && argc >= 4)
        fetch();
    else
        show_options(argv[0]);


    //in.name = argv[3];
    //in.title = argv[4]; 
    //in.next = 0;
    //out = add_1(&in, cl);
    //printf("out.arg: %d\n", (int)*out);

    return 0;
}




