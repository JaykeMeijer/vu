#include "add.h"


add_out *add_1_svc(param *in, struct svc_req *rqstp)
{
    static add_out out;

    printf("Recv request: %s %s\n", in->name, in->title);
    out = 0;

  return(&out);
}
