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

void print_header()
{
    printf("Content-type: text/html\n\n");

    printf("<!DOCTYPE HTML>\n");
    printf("<html>\n<head>\n<title>Index</title>\n</head>\n\n<body>\n");
}

void list_papers() 
{
    str_list *i;

    /* get the list from the server */
    i = list_1(NULL, cl);

    printf("<h1>Papers</h1>\n");

    if(i == NULL && i->number == -1) {
        printf("There are currently no papers stored.");
        return;
    }
    printf("<div id=\"papers\">\n");
    printf("<table>\n");
    printf("<th>Number</th>\n<th>Name</th>\n<th>Title</th>\n");

    /* traverse the list and print the number
     * name and title of the papers */
    while(i != NULL && i->number != -1) {
        printf("<tr>\n  <td><a href=paperview?id=%d>%d</a></td>\n  <td>%s</td>\n  <td>%s</td>\n</tr>\n", i->number, i->number, i->name, i->title);

        i = i->next;
    }
    
    printf("</table>\n");
    return;
}

int main(int argc, const char *argv[])
{
    cl = clnt_create(PAPERSERVER_ADDR, PAPER_SERVER, PAPER_VERSION, "tcp");

    if(cl == NULL) {
        perror("No RPC server found");
        exit(-1);
    }

    print_header();
    list_papers();
    printf("</body>\n</html>");

    return 0;
}
