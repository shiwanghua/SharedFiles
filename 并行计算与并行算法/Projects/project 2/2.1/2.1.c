#include <cfloat>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

typedef long long int lli;

double getTimeNs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)(ts.tv_sec * 1000000000 + ts.tv_nsec);
}

int main(int argc, char **argv) {
    int p;
    lli cnt = 0, n = 1, i;
    double x, y, pi, elapsed_time, PI = 3.1415926535, accuracy;
    if (argc != 3) {
        printf("%s misses parameters!\n", argv[0]);
        exit(1);
    }
    p = atoi(argv[1]);
    // n = atoi(argv[2]); // i 最多9个0
    // cout << n << endl;
    // n 最多18个0
    // int l = strlen(argv[2]) - 1;  // 输入的0的个数
    // int l2 = sizeof(argv[2]) - 1; // 每次都是7
    // cout << l << "  " << l2 << endl;
    // cout << argv[2] << endl;
    // n = 1;
    // while (l--)
    //     n *= 10;
    // cout << n << endl;  // 可以使得和输入一样
    // sscanf(argv[2], "%lld", &n); // 方法2
    // cout << n << endl;
    n = strtoll(argv[2], NULL, 10);
    // cout << n << endl;
    // n = strtold(argv[2], NULL);
    // cout << n << endl;

    elapsed_time = -getTimeNs();

    // 方法1：每次运行产生一样的结果，不随机
    // #pragma omp parallel num_threads(p)
    //     {
    //         unsigned seed = time(NULL);
    // #pragma omp parallel for private(i, x, y, seed) reduction(+ : cnt)
    // shared(n)
    //         for (i = 0; i < n; i++) {
    //             x = (double)rand_r(&seed) / RAND_MAX;
    //             y = (double)rand_r(&seed) / RAND_MAX;
    //             if (x * x + y * y <= 1.0)
    //                 cnt++;
    //         }
    //     }

    // 方法2：产生随机结果，但多线程会更慢，因为使用的同一个rand()
    // omp_set_num_threads(p);
    // #pragma omp parallel num_threads(p)
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

    // 方法3：产生随机结果 (精确度没这么高，但速度快些), 使用不同的生成器
    //omp_get_thread_num();
    const double maxValue = 10000000000;
    omp_set_num_threads(p);
#pragma omp parallel num_threads(p)
    {
        default_random_engine e(time(NULL)); //不加static也可以产生不同结果！
        uniform_real_distribution<double> u(0, maxValue);
#pragma omp parallel for private(x, y) reduction(+ : cnt) shared(n)
        for (int i = 0; i < n; i++) {
            x = u(e) / maxValue;
            y = u(e) / maxValue;
            // printf("%lf %lf\n", x, y);
            if (x * x + y * y <= 1.0)
                cnt++;
        }
    }

    // 方法4：先把随机数用单线程存到数组里！
    //     const double maxValue = 10000000000;
    //     double *ax, *ay;
    //     ax = (double *)malloc(n * sizeof(double));
    //     ay = (double *)malloc(n * sizeof(double));

    //     default_random_engine e(time(NULL));
    //     uniform_real_distribution<double> u(0, maxValue);
    //     for (i = 0; i < n; i++)
    //         ax[i] = u(e) / maxValue;
    //     for (i = 0; i < n; i++)
    //         ay[i] = u(e) / maxValue;

    //     elapsed_time = -getTimeNs();
    //     omp_set_num_threads(p);
    // #pragma omp parallel num_threads(p)
    //     {
    // #pragma omp parallel for private(i) reduction(+ : cnt) shared(n, ax, ay)
    //         for (i = 0; i < n; i++) {
    //             if (ax[i] * ax[i] + ay[i] * ay[i] <= 1.0)
    //                 cnt++;
    //         }
    //     }

    pi = (double)cnt / n * 4.0;
    elapsed_time += getTimeNs();
    accuracy = 1 - abs(pi - PI) / PI;
    printf("ThreadNum=%d, n=%lld, RunningTime= %5.6f s, pi≈ %lf, accuracy= "
           "%lf\n",
           p, n, elapsed_time / 1000000000, pi, accuracy);
    fflush(stdout);

    // free(ax), free(ay);
    return 0;
}
