#include <fstream>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
using namespace std;

// int **malloc_2D_matrix(int m, int n) {
//     int **matrix;
//     int *continuousSpace;
//     continuousSpace = (int *)malloc(m * n * sizeof(int));
//     matrix = (int **)malloc(m * sizeof(int *));
//     for (int i = 0; i < m; i++)
//         matrix[i] = &continuousSpace[i * n];
//     return matrix;
// }

// 二维数组当一维send？ // 核心问题
int main(int argc, char **argv) {
    double elapsed_time;
    int i, r, c, j, k, m, n, id, p, ones, d, nextr, nextc;
    // r遍历行，c遍历列，m行n列，ones是8个邻居中活着的1的个数，d遍历8个方向，nextr是下一方向的行号，nextj是下一方向的列号
    int *begin, *end, *num, *prefixSum;
    int x[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int y[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    // vector<vector<int>> state, next_state;
    // int **state, **next_state;
    // FILE *f;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (argc != 5) {
        if (!id) {
            printf("%s misses parameters again and again!\n", argv[0]);
            fflush(stdout);
        }
        MPI_Finalize();
        exit(1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    j = atoi(argv[1]); // 迭代 j 次
    k = atoi(argv[2]); // 每过 k 次输出一下状态
    m = atoi(argv[3]);
    n = atoi(argv[4]);

    // f = fopen("./init.txt", "r");
    // fscanf(f, "m=%d, n=%d\n", &m, &n);
    // 这种vector实现应该不连续
    // state.resize(m, vector<int>(n)); // m行n列
    // next_state.resize(m, vector<int>(n));
    // 生成动态二维连续空间数组方法1
    // state = malloc_2D_matrix(m, n);
    // next_state = malloc_2D_matrix(m, n);
    // 生成动态二维连续空间数组方法2
    int state[m][n];
    int next_state[m][n];
    // // 从文件读取
    // for (r = 0; r < m; ++r)
    //     for (c = 0; c < n; ++c)
    //         fscanf(f, "%d", &state[r][c]);
    // fclose(f);
    // 随机生成初始状态
    srand((unsigned)time(NULL));
    for (r = 0; r < m; ++r)
        for (c = 0; c < n; ++c)
            state[r][c] = (int)rand() % 2;
    begin = new int[p];
    end = new int[p];
    num = new int[p];
    prefixSum = new int[p];
    prefixSum[0] = 0;
    for (i = 0; i < p; i++) {
        begin[i] = i * m / p;         // 第i个进程从这一行开始处理
        end[i] = (i + 1) * m / p - 1; // 第i个进程处理到这一行为止
        if (end[i] >= m)
            end[i] = m - 1;
        num[i] = (end[i] - begin[i] + 1) * n; // 第i个进程每次处理的数目
        if (i)
            prefixSum[i] = prefixSum[i - 1] + num[i - 1];
    }

    // if (!id) {
    //     printf("id=%d, begin=%d, end=%d, num=%d, prefixSum=%d\n", i,
    //     begin[i],
    //            end[i], num[i], prefixSum[i]);
    //     fflush(stdout);
    // }
    for (i = 0; i < j; i++) {
        for (r = begin[id]; r <= end[id]; r++) {
            for (c = 0; c < n; c++) {
                ones = 0; // 1的个数（活着的个数）
                if (c == 0 || r == 0 || c == (n - 1) || r == (m - 1)) { //边界
                    for (int d = 0; d < 8; d++) {
                        nextr = r + x[d];
                        nextc = c + y[d];
                        if (nextr >= 0 && nextr < m && nextc >= 0 &&
                            nextc < n && state[nextr][nextc])
                            ones++;
                    }
                } else { // 不是边界时8个邻居不需要做边界检测
                    for (int d = 0; d < 8; d++)
                        if (state[r + x[d]][c + y[d]])
                            ones++;
                }
                if (state[r][c]) { // 当前活着
                    if (ones < 2 || ones > 3)
                        next_state[r][c] = 0; // 下一时间死了
                    else
                        next_state[r][c] = 1;
                } else { // 当前死了
                    if (ones == 3)
                        next_state[r][c] = 1;
                    else
                        next_state[r][c] = 0;
                }
            }
        }

        MPI_Allgatherv(next_state[begin[id]], num[id], MPI_INT, state[0], num,
                       prefixSum, MPI_INT, MPI_COMM_WORLD); // 同步状态
        if ((1 + i) % k == 0 && !id) {
            printf("loop%d:\n", i + 1);
            fflush(stdout);
            for (r = 0; r < m; ++r) {
                for (c = 0; c < n; ++c) {
                    printf("%d ", state[r][c]);
                    fflush(stdout);
                }
                printf("\n");
                fflush(stdout);
            }
            printf("\n");
            fflush(stdout);
        }
    }
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("Run Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }
    delete begin, end, num, prefixSum;
    // delete[] state[0];
    // delete[] state;
    // delete[] next_state[0];
    // delete[] next_state;
    MPI_Finalize();
    return 0;
}