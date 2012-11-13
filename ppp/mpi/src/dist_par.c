#include "mpi.h"
#include "tab.h"


void usage()
{
    printf("Usage: balblabla\n");
}

int distance(int **tab, int n, int lb, int ub)
{
    int i, j, dist = 0;

    for (i = lb; i < ub; i++)
        for (j = 0; j < n; j++)
            dist += tab[i][j];
    return dist;
}

int main(int argc, char *argv[])
{
    int m, n=1000, **tab, **tab_tmp, d, tot_d;
    int ierr, p, rank;
    int lb, ub;

    double wtime;

    //usage();

    ierr = MPI_Init(&argc, &argv);

    if(ierr != MPI_SUCCESS) {
        perror("MPI init error");
        exit(-1);
    }

    /* get size and rank */
    /* TODO error checking */
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(!rank)
        wtime = MPI_Wtime();

    /* send n to all processes */
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* initialize tabs on master process */
    if(!rank)
        init_tab(n, &m, &tab_tmp, 1);
    allo_tab_part(n, n/p, &tab);

    printf("[%d] 1\n", rank);

    /* send parts of tab to slaves */
    MPI_Bcast(&(tab[0][0]), n*n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&tab_tmp[0][0], n*n, MPI_INT, &tab[0][0], n*n/p, MPI_INT, 0, MPI_COMM_WORLD);

    printf("[%d] 2\n", rank);

    /* upper and lower bound */
    /* TODO covering all cases: n % p != 0 */
    lb = rank * n / p;
    ub = (rank + 1) * n / p;

    /* calculate total distance of the roads */
    d = distance(tab, n, lb, ub);

    /* TODO send distance to master + add the values */
    MPI_Reduce(&d, &tot_d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(!rank) { 
        wtime = MPI_Wtime() - wtime;
        printf("ASP took %f seconds\n", wtime);
    }

    /* print the total distance */
    //if(!rank)
    //    printf("Distance: %d\n", d);

    /* terminate MPI execution environment */
    MPI_Finalize();

    /* free the 2d road array */
    free_tab(tab, n);

    return 0;
}
