/*
 * Author: Swhua
 */

#include <mpi.h>
#include <stdio.h>
#define INTERVALS 1000000
int main(int argc, char **argv) {

    MPI_Init(NULL, NULL);
    MPI_Barrier(MPI_COMM_WORLD);
    double elapsed_time = -MPI_Wtime();

    int p, id, i, size, begin, k;
    double area, ysum = 0.0, xi, global_area = 0.0;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    k = INTERVALS % p; // 多余的k个平均分给前k个进程
    if (k) {
        if (id < k) { // 前k个进程每个多分一个数
            size = (INTERVALS + p) / p;
            begin = size * id;
        } else {
            size = INTERVALS / p;
            begin = size * id + k; // 前k个已分配完
        }
    } else { // 没多余的数
        size = INTERVALS / p;
        begin = size * id;
    }

    for (i = begin; i < begin + size; i++) {
        xi = 1.0 / INTERVALS * (i + 0.5);
        ysum += 4.0 / (1.0 + xi * xi);
    }

    area = ysum * (1.0 / INTERVALS);

    MPI_Reduce(&area, &global_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    elapsed_time += MPI_Wtime();
    MPI_Finalize();

    // printf("ID=%d, begin=%d, size=%d\n", id, begin, size);
    // fflush(stdout);
    if (!id) {
        printf("Area:      %2.20f\nTrue Value:3.14159265358979323846\nRun "
               "Time: %2.10f s\n",
               global_area, elapsed_time);
        fflush(stdout);
    }
}
