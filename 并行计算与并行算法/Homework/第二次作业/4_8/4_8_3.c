#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))

// 丢弃偶数，需要保证每块是偶数
int main(int argc, char *argv[]) {

    int id, p, index, prime, count, global_count = 0, first, i, n, proc0_size,
                                    size, low_value;
    int *boundary;
    bool *marked;

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

    marked = (bool *)malloc(size);
    if (marked == NULL) {
        printf("Cannot allocate enough memory: size=%d, low=%d\n", size,
               low_value);
        MPI_Finalize();
        exit(1);
    }
    memset(marked, 0, sizeof(bool) * size);

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
            while (marked[++index] || index % 2 == 0) // 是偶数就丢弃
                ;
            prime = index + 2;
        }
        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } while (prime * prime <= n);

    count = 0;
    if (!id) {
        // i=1代表3这个数
        for (i = 1; i < size - 2; i += 2) // 假设每块都是偶数个，除了第0块
            if (!marked[i] && !marked[i + 2])
                count++;
    } else {
        for (i = 0; i < size - 2; i += 2) // 第一个位置是奇数
            if (!marked[i] && !marked[i + 2])
                count++;
    }

    if (!id) {
        boundary = (int *)malloc(p * 2 * sizeof(int));
        memset(boundary, 0, sizeof(p * 2 * sizeof(int)));
        // 前p位存储p个进程的块的倒数第2个数是否是质数
        // 后p位存储p个进程的块的顺数第1个数是否是质数，0号进程的没有用
    }
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (id) {
        int firstodd = marked[0],
            lastodd = marked[size - 2]; // 转成int，因为不能传递bool变量！
        // 把边界结果发送给0号进程
        MPI_Send(&lastodd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&firstodd, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (!id) { // 边界情况
        // 接收其他进程的边界结果
        for (i = 1; i < p; i++) {
            MPI_Recv(boundary + i, 1, MPI_INT, i, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Recv(boundary + i + p, 1, MPI_INT, i, 1, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }

        boundary[0] = marked[size - 2];
        for (i = 0; i < p - 1; i++)
            if (!boundary[i] && !boundary[p + 1 + i])
                global_count++;
    }

    elapsed_time += MPI_Wtime();

    // printf("id=%d, low=%d, size=%d\n", id, low_value, size);
    // fflush(stdout);

    if (!id) {
        printf("%d times that two consecutive odd integers are both prime.\n",
               global_count);
        fflush(stdout);
        printf("Total elapsed time: %10.6f\n", elapsed_time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}

// 1 2 4