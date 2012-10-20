#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "add.h"

int main(int argc, char **argv) {
    CLIENT *cl;
    param in, *tmp;
    add_out *out;

    if (argc<4) { 
        printf("Usage: %s <machine> <int1> <int2> <int3> ...\n\n", argv[0]);
        return 1;
    }
    cl = clnt_create(argv[1], ADD_PROG, ADD_VERS, "udp");

    tmp = &in;
    in.arg = strdup(argv[2]);
    
    printf("Arg: ***%s***\n",in.arg);

    argc -= 3;
    argv += 3;

    while (argc>0) {
        tmp->next = (struct param*) malloc(sizeof(struct param));
        tmp = tmp->next;
        tmp->arg = strdup(argv[0]);

        printf("Arg: ***%s***\n",tmp->arg);

        argc--;
        argv++;
    }
    tmp->next = NULL;

    out = add_1(&in, cl);
    
    if (out==NULL) {
        printf("Error: %s\n",clnt_sperror(cl,argv[1]));
    }
    else {
        printf("We received the result: %d\n",*out);
    }
    return 0;
}
