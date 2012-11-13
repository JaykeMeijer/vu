#include "paper.h"

struct str_server {
    int     number;
    char    *name;
    char    *title;
    struct  paper  p;
    struct  str_server   *next;
};

struct str_server head;
struct str_server *tail = &head;

void free_str_server(struct str_server *node)
{
    free(node->name);
    free(node->title);
    //free(node->next);
}


int_out *add_1_svc(str_add *in, struct svc_req *rqstp)
{
    static int_out out;
    struct str_server *tmp;

    tmp = (struct str_server*) malloc(sizeof(struct str_server));

    if(tmp == NULL) {
        perror("malloc error");
        out = -1;
        return(&out);
    }

    /* set number */
    out = tail->number + 1;
    tmp->number = out;

    /* copy name and title */
    tmp->name = strdup(in->name);
    tmp->title = strdup(in->title);

    /* copy paper (binary data) */
    tmp->p.len = in->p.len;
    tmp->p.val = malloc(sizeof(char) * in->p.len);
    memcpy(tmp->p.val, &in->p.val, in->p.len);

    /* pointers should always be initialized */
    tmp->next = NULL;

    /* advance the tail */
    tail->next = tmp;
    tail = tail->next;

    return(&out);
}

int_out *remove_1_svc(int* num, struct svc_req *rqstp)
{
    struct str_server *pred = &head,
                      *curr = head.next;
    static int_out v = 0;
    

    /* no papers stored yet */
    if(curr == NULL)
        return(&v);

    /* find the paper corresponding with
     * the given number: num */
    do {
        if(curr->number == *num) {
            /* remove paper by advancing next pointer */
            pred->next = pred->next-> next;

            /* and freeing the current node */
            free_str_server(curr);
            break;
        }
    } while((pred = curr) && (curr = curr->next) != NULL);

    return(&v);
}

str_list *list_1_svc(void *v, struct svc_req *rqstp)
{
    static struct str_list out;
    struct str_list *tmp = &out;
    struct str_server *curr = head.next;

    /* no papers stored yet */
    if(curr == NULL) {
        out.number = -1;
        out.name = "";
        out.title = "";
        return(&out);
    }

    while(1) {
        tmp->number = curr->number;

        tmp->name = malloc(sizeof(char) * 100);
        tmp->title = malloc(sizeof(char) * 100);
        tmp->name = strdup(curr->name);
        tmp->title = strdup(curr->title);

        if(curr->next == NULL) {
            tmp->next = NULL;
            break;
        }
        tmp->next = (struct str_list*) malloc(sizeof(struct str_list));
        tmp = tmp->next;
        curr = curr->next;
    }
    /* reset the pointer to start of */
    tmp = &out;
    return(&out);
}

/* get info of the paper with given number */
str_info *info_1_svc(int *number, struct svc_req *rqstp)
{
    static str_info out;
    struct str_server *curr = &head;

    /* traverse the linked list */
    while((curr = curr->next) != NULL) {
        /* do we have the correct paper */
        if(curr->number == *number) {
            out.number = curr->number;
            out.name = curr->name;
            out.title = curr->title;

            return(&out);
        }
    }
    /* paper not found send empty structure */
    out.number = -1;
    out.name = "";
    out.title = "";

    return(&out);
}

/* returns the paper with the corresponding number */
paper *fetch_1_svc(int *number, struct svc_req *rqstp)
{
    static paper out;
    struct str_server *curr = &head;

    while((curr = curr->next) != NULL) {
        if(curr->number == *number) {
            /* copy paper (binary data) */
            out.len = curr->p.len;
            out.val = malloc(sizeof(char) * curr->p.len);
            memcpy(out.val, &curr->p.val, curr->p.len);

            return(&out);
        }
    }
    out.len = 0;
    out.val = "";
    
    return(&out);
}

