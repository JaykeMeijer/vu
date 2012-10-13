struct param {
  string        arg<32>;
  struct param *next;
};

typedef int add_out;

program ADD_PROG {
  version ADD_VERS {
    add_out ADD(param) = 1;  /* Procedure nb */
  } = 1;                     /* Version nb */
} = 0x20001234;              /* Program number */
