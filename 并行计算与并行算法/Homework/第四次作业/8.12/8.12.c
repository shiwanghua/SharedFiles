#include <float.h>
#include <mpi.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

void print_root(int **root, int low, int high) {
    printf("Root spanning %d-%d is %d\n", low, high, root[low][high + 1]);
    fflush(stdout);
    if (low < root[low][high + 1] - 1) {
        print_root(root, low, root[low][high + 1] - 1);
    }
    if (root[low][high + 1] < high - 1) {
        print_root(root, root[low][high + 1] + 1, high);
    }
}

void alloc_matrix(void ***a, int m, int n, int size) {
    int i;
    void *storage;
    storage = (void *)malloc(m * n * size);
    *a = (void **)malloc(m * sizeof(void *));
    for (i = 0; i < m; i++) {
        //(*a)[i] = (float *)storage + i * n * size; // VS
        (*a)[i] = storage + i * n * size; // 服务器环境
    }
}

// Baase and Van Gelder
//  构造最优二叉查找树，最小化查找次数，每个节点都是一个关键字，每个关键字的概率已知
int main(int argc, char *argv[]) {
    int low, high, i, j, r, l, bestroot, n, id, p, **root;
    float bestcost, rcost, **cost;
    double elapsed_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    // n = 8;
    // float pro[8] = {0.16, 0.13, 0.06, 0.08, 0.07, 0.17, 0.05, 0.28};
    // float pro_sum[9] = {0, 0.16, 0.29, 0.35, 0.43, 0.50, 0.67, 0.72, 1.0};
    n = atoi(argv[1]);
    float *pro = (float *)malloc(n * sizeof(float));
    float *pro_sum = (float *)malloc((n + 1) * sizeof(float)); // 前缀和
    if (id == 0) {
        float sum = 0.0;
        default_random_engine e(time(0));
        uniform_real_distribution<double> u(0.0, 1.0);
        for (i = 0; i < n; i++) {
            pro[i] = u(e);
            sum += pro[i];
        }
        for (i = 0; i < n; i++)
            pro[i] = pro[i] / sum;
        pro_sum[0] = 0.0;
        for (i = 1; i <= n; i++) // 前缀和
            pro_sum[i] = pro_sum[i - 1] + pro[i - 1];
    }

    MPI_Bcast(pro, n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(pro_sum, n + 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    alloc_matrix((void ***)&cost, n + 1, n + 1, sizeof(float));
    alloc_matrix((void ***)&root, n + 1, n + 1, sizeof(int));

    for (l = 0; l <= n; l++) { // 计算长度为l的连续子序列，l最多是n
        for (low = id; low + l <= n; low += p) {
            high = low + l;
            if (low == high) { // l为0即只有1个结点
                cost[low][low] = 0.0;
                root[low][low] = low;
            } else {
                bestcost = MPI_FLOAT;
                for (r = low; r < high; r++) { // 遍历 l 种根结点
                    rcost = cost[low][r] + cost[r + 1][high] + pro_sum[high] -
                            pro_sum[low];
                    if (rcost < bestcost) {
                        bestcost = rcost;
                        bestroot = r;
                    }
                }
                cost[low][high] = bestcost;
                root[low][high] = bestroot;
            }
        }
        for (low = 0; low + l <= n; low++) { // 共享长为 l 的最优解
            high = low + l;
            MPI_Bcast(&cost[low][high], 1, MPI_FLOAT, low % p, MPI_COMM_WORLD);
            MPI_Bcast(&root[low][high], 1, MPI_INT, low % p, MPI_COMM_WORLD);
        }
    }

    if (!id)
        print_root(root, 0, n - 1);

    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("\nRun Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
