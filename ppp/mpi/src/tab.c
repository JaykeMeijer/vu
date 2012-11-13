#include "tab.h"

int allo_tab_part(int n, int rows, int ***tabptr)
{
    int i;

    int *p = (int *)malloc(n * rows * sizeof(int));
    if(!p)
        return -1;

    (*tabptr) = (int **)malloc(n * sizeof(int *));
    if(!(*tabptr)) {
        free(p);
        return -1;
    }

    for(i = 0; i < rows; i++)
        (*tabptr)[i] = &(p[i * n]);

    return 0;
}

int allocate_tab(int n, int ***tabptr)
{
    int i;

    /* allocate the n*m contiguous items */
    int *p = (int *)malloc(n * n * sizeof(int));
    if (!p)
        return -1;

    /* allocate the row pointers into the memory */
    (*tabptr) = (int **)malloc(n * sizeof(int *));
    if (!(*tabptr)) {
       free(p);
       return -1;
    }

    /* set up the pointers into the contiguous memory */
    for(i = 0; i < n; i++) 
       (*tabptr)[i] = &(p[i * n]);

    return 0;
}

void init_tab(int n, int *mptr, int ***tabptr, int oriented)
{
    int **tab;
    int i, j, m=n*n;

    if(allocate_tab(n, tabptr) == -1)
        exit(42);

    tab = *tabptr;

    for(i = 0; i < n; i++) {
        tab[i][i] = 0;

        for(j = 0; j < i; j++) {
            tab[i][j] = 1+(int)((double)MAX_DISTANCE*rand()/(RAND_MAX+1.0));
            if(oriented) 
                tab[j][i] = 1+(int)((double)MAX_DISTANCE*rand()/(RAND_MAX+1.0));
            else 
                tab[j][i] = tab[i][j];
            if(tab[i][j]==MAX_DISTANCE)
                m--;
            if(tab[j][i]==MAX_DISTANCE)
                m--;
        }
    }
    *mptr = m;
}

int read_tab(char *INPUTFILE, int *nptr, int *mptr, int ***tabptr, int *optr)
{
    int **tab;
    int i,j,n,m;
    int source, destination, weight;
    FILE* fp; 
    int bad_edges=0, oriented=0;

    fp = fopen(INPUTFILE, "r");
    fscanf(fp, "%d %d %d \n", &n, &m, &oriented);

#ifdef VERBOSE
    printf("%d %d %d\n", n,m,oriented);
#endif

        tab =(int **)malloc(n * sizeof(int *));
        if(tab == (int **)0) {
            fprintf(stderr,"cannot malloc distance table\n");
            exit(42);
        }

        for(i = 0; i < n; i++) {
            tab[i] =(int *)malloc(n * sizeof(int));
        if(tab[i] == (int *)0) {
            fprintf(stderr,"cannot malloc distance table\n");
            exit(42);
        }
        
        for(j = 0; j < n; j++) {
            tab[i][j] =(i == j) ? 0 : MAX_DISTANCE;
        }
    }

    while(!feof(fp)) {
        fscanf(fp, "%d %d %d \n", &source, &destination, &weight);
        if(!oriented)
            if(tab[source-1][destination-1] < MAX_DISTANCE) 
                bad_edges++;
            else {
                tab[source-1][destination-1]=weight;
                tab[destination-1][source-1]=weight;
            }
        else 
            tab[source-1][destination-1]=weight;
    }
    fclose(fp);
#ifdef VERBOSE 
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) 
            printf("%5d", tab[i][j]);
        printf("\n");
    }
#endif

    *tabptr=tab;
    *nptr=n;
    *mptr=m;
    *optr=oriented;
    return bad_edges; 
}

void free_tab(int **tab, int n)
{
    free(tab[0]);
    free(tab);
}

void print_tab(int **tab, int n)
{
    int i, j;

    for(i=0; i<n; i++) {
        for(j=0; j<n; j++)
            printf("%3d ", tab[i][j]);
        printf("\n");
    }
}
