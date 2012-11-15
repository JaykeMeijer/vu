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
    int lb, ub, *tab_1d;
    int *send, *recv, *ptr;

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
    if(!rank) {
        init_tab(n, &m, &tab_tmp, 1, &send);
    }
    allo_tab_part(n, n/p, &tab, &recv);

    /* send parts of tab to slaves */
    MPI_Scatter(send, n/p, MPI_INT, recv, n/p, MPI_INT, 0, MPI_COMM_WORLD);

    /* upper and lower bound */
    /* TODO covering all cases: n % p != 0 */
    lb = rank * n / p;
    ub = (rank + 1) * n / p;

    ub = 0;

    printf("10\n");
    /* calculate total distance of the roads */
    for(lb = 0; lb < n/p; lb++)
        ub += recv[lb];
    printf("ub: %d\n", ub);
    //d = distance(recv, n, lb, ub);

    printf("d: %d\n", d);
    /* TODO send distance to master + add the values */
    //MPI_Reduce(&d, &tot_d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /*if(!rank) {
        wtime = MPI_Wtime() - wtime;
        printf("ASP took %f seconds\n", wtime);
    }*/

    /* print the total distance */
    //if(!rank)
    //    printf("Distance: %d\n", d);

    /* terminate MPI execution environment */
    //MPI_Finalize();

    /* free the 2d road array */
    //free_tab(tab, n);
    //free_tab(tab_tmp, n);

    return 0;
}
