#include <sys/time.h>
#include "tab.h"

void usage()
{
    printf("Usage: balblabla\n");
}

int distance(int **tab, int n)
{
    int i, j, dist = 0;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dist += tab[i][j];
    return dist;
}

int main(int argc, const char *argv[])
{
    int m, n=1000, **tab, d;
    double time;
    struct timeval start, end;

    //usage();
    
    gettimeofday(&start, 0) ;

    /* initialize tabs with random shit */
    init_tab(n, &m, &tab, 1);

    /* calculate total distance of the roads */
    d = distance(tab, n);

    gettimeofday(&end, 0);

    /* print the total distance */
    //printf("Distance: %d\n", d);

    time = (end.tv_sec + (end.tv_usec / 1000000.0)) -                     
    (start.tv_sec + (start.tv_usec / 1000000.0));                     

    printf("ASP took %f seconds\n", time); 


    /* free the 2d road array */
    free_tab(tab, n);

    return 0;
}

