#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "add.h"

CLIENT *cl;

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

void add(const char **arg)
{
    FILE *f;
    long size;
    int number;
    str_add paper;

    paper.name = (char *)arg[0];
    paper.title = (char *)arg[1];

    f = fopen(arg[2], "rb");
    if(f == NULL) {
        printf("File %s not found", arg[2]);
        exit(-1);
    }
    /* get size of file */
    if(fseek(f, 0L, SEEK_END) == -1) {
        perror("fseek failed");
        exit(-1);
    }
    size = ftell(f);
    if(size < 0) {
        perror("ftell failed");
        exit(-1);
    }
    rewind(f);

    paper.p.len = size;
    paper.p.val = malloc(sizeof(char) * size);
    if(paper.p.val == NULL) {
        perror("malloc failed");
        exit(-1);
    }
    fread(paper.p.val, 1, size, f);
    fclose(f);

    number = *add_1(&paper, cl);
    printf("%d\n", number);

    return;
}
void remove_paper(char* num)
{
    int number;

    number = atoi(num);
    remove_1(&number, cl);

    return;
}
void list()
{ 
    str_list *i;
    
    i = list_1((void*) NULL, cl);
    printf("list: %p %p\n", i, i->next);

    while((i = i->next) != NULL) {
        printf("%d\t%s\t%s\n", i->number, i->name, i->title);
    }
    return;
}
void info(char* num) 
{
    str_info in;
    int number;

    number = atoi(num);
    in = *info_1(&number, cl);

    printf("%d\t%s\t%s\n", in.number, in.name, in.title);
    

    return;
}
void fetch() { return; }


int main(int argc, const char *argv[])
{
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
        remove_paper((char *)argv[3]);
    else if(!strcmp(argv[2], "list"))
        list();
    else if(!strcmp(argv[2], "info"))
        info((char *)argv[3]);
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




