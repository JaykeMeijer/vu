#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../paper/paper.h"
#include "../setup.h"

CLIENT *cl;

/* checks whether a string represends a 
 * valid number
 *
 * return 0 if not and 1 otherwise
 */
int is_a_number(const char *str, int *num)
{
    char *end;
    *num = strtol(str, &end, 10);

    if (end == str || *end != '\0' || errno == ERANGE)
        return 0;
    else
        return 1;
}

void show_paper(int id)
{
    int i;
    paper *p = NULL;
    
    /* get the paper */
    p = fetch_1(&id, cl);
    
    /* print each character of the binary
     * file. NOTE not all characters actually
     * print something on the screen */
    printf("Content-type: application/octet-stream\n");
    printf("Content-Length: %ld\n\n", p->len);

    for(i = 0; i < p->len; i++)
        printf("%c", p->val[i]);
}

int main(int argc, const char *argv[])
{
    char *query;
    int number;


    cl = clnt_create(PAPERSERVER_ADDR, PAPER_SERVER, PAPER_VERSION, "tcp");

    if(cl == NULL) {
        perror("No RPC server found");
        exit(-1);
    }
    
    query = getenv("QUERY_STRING");
    if(is_a_number(query+3, &number))
        show_paper(number);
    else {
        printf("Content-type: text/html\n\n");
        printf("Paper not found");
    }
        

    return 0;
}
