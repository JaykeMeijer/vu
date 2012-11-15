#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX_DISTANCE 256

int allo_tab_part(int n, int rows, int ***tabptr, int **p);
int allocate_tab(int n, int ***tabptr, int **p);
void init_tab(int n, int *mptr, int ***tabptr, int oriented, int **p);
int read_tab(char *INPUTFILE, int *nptr, int *mptr, int ***tabptr, int *optr, int **p);
void free_tab(int **tab, int n);
void print_tab(int **tab, int n);
