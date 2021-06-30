#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

bool isPrime(int n) {
    for (int i = 2; i <= (int)sqrt((double)n); ++i)
        if (n % i == 0)
            return false;
    return true;
}

int main(int argc, char *argv[]) {

    int id, p, prime, global_count = 0, i, j, n, sqrtn, ith = 0;
    bool *mark, *global_mark;

    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    double elapsed_time = -MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 2) {
        if (!id)
            printf("%s misses parameter 'n'!\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    n = atoi(argv[1]); // 1000000
    sqrtn = (int)sqrt((double)n);

    mark = (bool *)malloc(n + 1);
    memset(mark, 0, sizeof(bool) * (n + 1));
    if (mark == NULL) {
        printf("Mark Memory Allocation Fail\n");
        MPI_Finalize();
        exit(1);
    }

    // 单独处理2，因为2是偶数
    if (!id)
        for (i = 4; i <= n; i += 2)
            mark[i] = 1;

    for (i = 3; i <= sqrtn; i += 2) {
        if (isPrime(i)) {
            ith++; // 质数的编号，2是0号，3是1号
            if (ith % p == id)
                // for (j = i * i; j <= n; j += i)
                for (j = i << 1; j <= n; j += i)
                    mark[j] = 1;
        }
    }

    if (!id) {
        global_mark = (bool *)malloc(n + 1);
        if (global_mark == NULL) {
            printf("Global Mark Memory Allocation Fail\n");
            MPI_Finalize();
            exit(1);
        }
        memset(global_mark, 0, sizeof(bool) * (n + 1));
    }
    MPI_Reduce(mark, global_mark, n + 1, MPI_CHAR, MPI_LOR, 0, MPI_COMM_WORLD);

    if (!id) {
        global_count = 1;
        for (i = 3; i <= n; i += 2)
            if (!global_mark[i])
                global_count++;
    }

    elapsed_time += MPI_Wtime();

    if (!id) {
        printf("%d primes in (1-%d).\nTotal elapsed time: %3.9f s\n",
               global_count, n, elapsed_time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}