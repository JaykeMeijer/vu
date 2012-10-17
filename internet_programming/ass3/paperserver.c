#include "add.h"

struct str_server head;
struct str_server *tail = &head;

int_out *add_1_svc(str_add *in, struct svc_req *rqstp)
{
    static int_out out;
    str_server *tmp;

    tmp = malloc(sizeof(str_server));

    if(tmp == NULL) {
        perror("malloc error");
        out = -1;
        return(&out);
    }
    out = tail->number + 1;
    tmp->number = out;
    tmp->name = in->name;
    tmp->title = in->title;
    tmp->p = in->p;
    tmp->next = NULL;

    tail->next = tmp;
    tail = tail->next;

    printf("head == tail: %d\n", &head == tail);
    printf("head.next == tail: %d\n", head.next == tail);
    printf("head.next: %d\t%s\t%s\n", head.next->number, head.next->name, head.next->title);
    printf("tail: %d\t%s\t%s\n", tail->number, tail->name, tail->title);

    return(&out);
}

int_out *remove_1_svc(int* num, struct svc_req *rqstp)
{
    struct str_server *pred = &head,
                      *curr = head.next;
    static int_out v = 0;

    if(curr == NULL)
        return(&v);

    do {
        if(curr->number == *num) {
            pred->next = curr-> next;
            free(curr);
            break;
        }
    } while((pred = curr) && (curr = curr->next) != NULL);

    return(&v);
}

str_list *list_1_svc(void *v, struct svc_req *rqstp)
{
    static struct str_list out,
                           *tmp = &out;
    struct str_server *curr = &head;

    while((curr = curr->next) != NULL) {
        tmp->next = malloc(sizeof(struct str_info));
        tmp = tmp->next;
        printf("_\n");
        tmp->number = curr->number;
        tmp->name = curr->name;
        tmp->title = curr->title;
    }
    tmp->next = NULL;

    return(&out);
}

str_info *info_1_svc(int *number, struct svc_req *rqstp)
{
    static str_info out;
    str_server *curr = &head;

    printf("%s\n", head.next->name);
    
    while((curr = curr->next) != NULL) {
        if(curr->number == *number) {
            printf("%d\t%s\t%s\n", curr->number, curr->name, curr->title);
            out.number = curr->number;
            out.name = curr->name;
            out.title = curr->title;
            break;
        }
    }
    return(&out);
}




