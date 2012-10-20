#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "add.h"

CLIENT *cl;

/* displays the available command-line options available */
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

/* checks whether a string represends a 
 * valid number
 *
 * return 0 if not and 1 otherwise
 */
int is_a_number(const char *str)
{
    char *end;
    strtol(str, &end, 10);
    
    if (end == str || *end != '\0' || errno == ERANGE)
        return 0;
    else
        return 1;
}

/* sends a paper to the server which
 * should be added */
void add(const char **arg)
{
    FILE *f;
    long size;
    int number;
    str_add paper;

    /* store name and title */
    paper.name = (char *)arg[0];
    paper.title = (char *)arg[1];

    f = fopen(arg[2], "rb");
    if(f == NULL) {
        printf("File %s not found", arg[2]);
        exit(-1);
    }
    
    /* set file position indicator
     * to the end of the file */
    if(fseek(f, 0L, SEEK_END) == -1) {
        perror("fseek failed");
        exit(-1);
    }

    /* get the size of the file */ 
    size = ftell(f);
    if(size < 0) {
        perror("ftell failed");
        exit(-1);
    }
    
    /* set the file position indicator
     * to the start of the file */
    rewind(f);

    /* store the paper size */
    paper.p.len = size;

    /* read file and store it */
    paper.p.val = malloc(sizeof(char) * size);
    if(paper.p.val == NULL) {
        perror("malloc failed");
        exit(-1);
    }
    fread(paper.p.val, 1, size, f);
    fclose(f);

    /* give rpc add request to server */
    number = *add_1(&paper, cl);
    printf("%d\n", number);

    return;
}

/* remove a paper from the server */
void remove_paper(char* num)
{
    int number;

    /* convert string to number */
    number = atoi(num);

    /* give rpc remove request to server */
    remove_1(&number, cl);

    return;
}

/* list the information of all papers
 * currently stored at the server */
void list()
{ 
    str_list *i;
    
    /* get the list from the server */
    i = list_1(NULL, cl);

    /* traverse the list and print the number
     * name and title of the papers */
    while(i != NULL) {
        printf("%d\t%s\t%s\n", i->number, i->name, i->title);
        i = i->next;
    }
    return;
}

/* get the information of a specific paper */
void info(char* num) 
{
    str_info *in = NULL;
    int number;

    /* convert string to number */
    number = atoi(num);

    /* get the info from the server */
    in = info_1(&number, cl);

    /* print if a paper is found */
    if(in->number != -1)
        printf("%d\t%s\t%s\n", in->number, in->name, in->title);

    return;
}

/* fetch a paper from the server */
void fetch(char* num)
{
    paper *p = NULL;
    int i, number;

    /* convert string to number */
    number = atoi(num);

    /* get the paper */
    p = fetch_1(&number, cl);

    /* print each character of the binary
     * file. NOTE not all characters actually
     * print something on the screen */
    for(i = 0; i < p->len; i++)
        printf("%c", p->val[i]);
    
    return;
}


int main(int argc, const char *argv[])
{
    if(argc < 3)
        show_options(argv[0]);

    /* connect to the sun rpc server */
    cl = clnt_create(argv[1], PAPER_SERVER, PAPER_VERSION, "tcp");
    if(cl == NULL) {
        perror("No RPC server found");
        exit(-1);
    }

    /* execute command corresonding to the input
     * check on enough arguments and if given
     * numbers can be converted */
    if(!strcmp(argv[2], "add") && argc >= 6)
        add(argv+3);
    else if(!strcmp(argv[2], "remove") && argc >= 4 && is_a_number(argv[3]))
        remove_paper((char *)argv[3]);
    else if(!strcmp(argv[2], "list"))
        list();
    else if(!strcmp(argv[2], "info") && argc == 4 && is_a_number(argv[3]))
        info((char *)argv[3]);
    else if(!strcmp(argv[2], "fetch") && argc == 4 && is_a_number(argv[3]))
        fetch((char *)argv[3]);
    else
        show_options(argv[0]);

    return 0;
}
