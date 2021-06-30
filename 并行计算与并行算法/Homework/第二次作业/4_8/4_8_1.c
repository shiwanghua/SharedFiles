#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))

int main(int argc, char *argv[]) {

    int id, p, index, prime, count, global_count, first, i, n, proc0_size, size,
        low_value;

    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    double elapsed_time = -MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 2) {
        if (!id)
            printf("%s misses parameter!\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    n = atoi(argv[1]); // 1000000

    low_value = 2 + BLOCK_LOW(id, p, n - 1);
    size = BLOCK_LOW((id) + 1, p, n - 1) - low_value + 2;
    proc0_size = (n - 1) / p;
    if ((2 + proc0_size) < (int)sqrt((double)n)) {
        if (!id)
            printf(
                "Too many processes.\n"); // 保证sieving primes全在0号进程被找到
        MPI_Finalize();
        exit(1);
    }

    bool *marked = (bool *)malloc(size);
    if (marked == NULL) {
        printf("Cannot allocate enough memory: size=%d, low=%d\n", size,
               low_value);
        MPI_Finalize();
        exit(1);
    }

    if (!id)
        index = 0;
    prime = 2;
    do {
        if (prime * prime > low_value)
            first = prime * prime - low_value;
        else {
            if (!(low_value % prime))
                first = 0;
            else
                first = prime - (low_value % prime);
        }
        for (i = first; i < size; i += prime)
            marked[i] = 1;
        if (!id) { // 找下一个sieving primes
            while (marked[++index])
                ;
            prime = index + 2;
        }
        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } while (prime * prime <= n);

    count = 0;
    for (i = 0; i < size; i++)
        if (!marked[i])
            count++;
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    printf("low=%d, size=%d\n", low_value, size);
    fflush(stdout);
    if (!id) {
        printf("%d primes are less than or equal to %d.\n", global_count, n);
        printf("Total elapsed time: %10.6f\n", elapsed_time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}