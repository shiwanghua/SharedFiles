#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int main(int argc, char *argv[]) {

    int id, p, prime, global_count = 0, i, j, n, sqrtn, ith = 0;
    bool *mark, *global_mark;
    static int s = -1;
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    double elapsed_time = -MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    s = id;
    j = 0;
    for (int i = 0; i < 1000000000; i++)
        j += 1;
    printf("%d s=%d\n", id, s);
    fflush(stdout);
    MPI_Finalize();
    return 0;
}