#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX_DISTANCE 256

int allo_tab_part(int n, int rows, int ***tabptr);
int allocate_tab(int n, int ***tabptr);
void init_tab(int n, int *mptr, int ***tabptr, int oriented);
int read_tab(char *INPUTFILE, int *nptr, int *mptr, int ***tabptr, int *optr);
void free_tab(int **tab, int n);
void print_tab(int **tab, int n);
