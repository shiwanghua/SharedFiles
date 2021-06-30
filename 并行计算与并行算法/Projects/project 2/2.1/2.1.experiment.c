#include <cfloat>
#include <ctime>
#include <mpi.h>
#include <omp.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

typedef long long int lli;

double getTimeNs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)(ts.tv_sec * 1000000000 + ts.tv_nsec);
}

int main(int argc, char **argv) {
    int p, id; // p 是mpi进程数，实验次数！
    lli cnt = 0, n, i;
    double x, y, pi, elapsed_time, PI = 3.1415926535, accuracy;
    if (argc != 2) {
        printf("%s wrong parameters!\n", argv[0]);
        exit(1);
    }
    n = atoi(argv[1]);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);

    elapsed_time = -getTimeNs();

    // // 方法1：产生随机结果
    // #pragma omp parallel num_threads(id + 1)
    //     {
    //         srand((unsigned int)time(NULL));
    // #pragma omp parallel for private(i, x, y) reduction(+ : cnt) shared(n)
    //         for (i = 0; i < n; i++) {
    //             x = (double)rand() / RAND_MAX;
    //             y = (double)rand() / RAND_MAX;
    //             if (x * x + y * y <= 1.0)
    //                 cnt++;
    //         }
    //     }

#pragma omp parallel num_threads(p)
    {
        default_random_engine e(time(NULL)); // 不加static也可以产生不同结果！
        uniform_real_distribution<double> u(0, 100000000);
#pragma omp parallel for private(i, x, y) reduction(+ : cnt) shared(n)
        for (i = 0; i < n; i++) {
            x = u(e) / 100000000;
            y = u(e) / 100000000;
            // printf("%lf %lf\n", x, y);
            if (x * x + y * y <= 1.0)
                cnt++;
        }
    }

    pi = (double)cnt / n * 4.0;

    elapsed_time += getTimeNs();

    accuracy = 1 - abs(pi - PI) / PI;
    printf("numThreads=%d, Running Time = %5.6f s, pi ≈ %lf, accuracy = %lf\n",
           id + 1, elapsed_time / 1000000000, pi, accuracy);
    fflush(stdout);
    MPI_Finalize();
    return 0;
}
