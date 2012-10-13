#include "add.h"

add_out *add_1_svc(param *in, struct svc_req *rqstp) {
  static add_out out;
  fprintf(stderr,"Recv request: ");
  out = 0;

  do
  {
    fprintf(stderr,"+ %s ",in->arg);
    out += atol(in->arg);
    in = in->next;
  } while (in);

  fprintf(stderr,"\n");

  return(&out);
}
