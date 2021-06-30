/*
 * Author: raphael hao
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    // Initialize the MPI environment. The two arguments to MPI Init are not
    // currently used by MPI implementations, but are there in case future
    // implementations might need the arguments.
    MPI_Init(NULL, NULL);

    printf("%d\n", (int)log(0));
    printf("%d\n", (int)(log(15) / log(2)));
    printf("%d\n", (int)(log(5) / log(2)));
    MPI_Finalize();
}
