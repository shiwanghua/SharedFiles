//#pragma comment(lib,"msmpi.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <random>
#include <string.h>
#include <time.h>
#include <vector>

using namespace std;

double **malloc_2d_matrix(int m, int n) {
    double **matrix;
    double *continuousspace;
    continuousspace = (double *)malloc(m * n * sizeof(double));
    matrix = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
        matrix[i] = &continuousspace[i * n];
    return matrix;
}

int main(int argc, char **argv) {
    double elapsed_time;
    int i, j, r, c, m, n, k, id, p;
    // r遍历行，c遍历列，m行n列, id进程负责k列数据
    int *begin, *end, *num;
    double *a_row, *b_vector, *result, **a, *b,
        *ab; // a_row,b_vecctor: 读取时暂存数组;
             // a是部分矩阵，b是部分向量，ab是部分结果

    // FILE* f;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    // f = fopen("data/matrix_vector.txt", "r");
    // fscanf(f, "m=%d, n=%d\n", &m, &n);

    if (!id) {
        a_row = (double *)malloc(sizeof(double) * n);
        b_vector = (double *)malloc(sizeof(double) * n);
        result = (double *)malloc(sizeof(double) * m);
        memset(result, 0, sizeof(double) * m);
    } else { // 避免编译器认为没初始化就使用
        // error2: 这三个指针不能指向同一个地址，不然最后free会释放多次
        a_row = (double *)malloc(1);
        b_vector = (double *)malloc(1);
        result = (double *)malloc(1);
    }

    // error1: 这几个数组不能放在 if(!id) 里，MPI_Scatterv里别的进程也要用
    begin = (int *)malloc(sizeof(int) * p);
    end = (int *)malloc(sizeof(int) * p);
    num = (int *)malloc(sizeof(int) * p);
    for (i = 0; i < p; i++) {
        begin[i] = i * n / p; // 第i个进程从这一列开始处理 0, 3, 6
        end[i] = (i + 1) * n / p - 1; // 第i个进程处理到这一列为止 2, 5, 9
        num[i] = end[i] - begin[i] + 1; // 第i个进程每行处理的列数 3, 3, 4
    }

    k = n * (id + 1) / p - n * id / p;
    a = malloc_2d_matrix(m, k);
    b = (double *)malloc(sizeof(double) * k);
    ab = (double *)malloc(sizeof(double) * m);

    default_random_engine e(time(0));
    uniform_real_distribution<double> u(-1.0, 1.0);
    for (r = 0; r < m; ++r) {
        if (!id)
            for (c = 0; c < n; ++c)
                // fscanf(f, "%lf", &a_row[c]);
                a_row[c] = u(e);
        MPI_Scatterv(a_row, num, begin, MPI_DOUBLE, a[r], num[id], MPI_DOUBLE,
                     0, MPI_COMM_WORLD);
    }

    if (!id)
        for (c = 0; c < n; ++c)
            // fscanf(f, "%lf", &b_vector[c]);
            b_vector[c] = u(e);
    MPI_Scatterv(b_vector, num, begin, MPI_DOUBLE, b, num[id], MPI_DOUBLE, 0,
                 MPI_COMM_WORLD);
    // fclose(f);

    for (i = 0; i < m; i++) {
        ab[i] = 0.0;
        for (j = 0; j < k; j++)
            ab[i] += a[i][j] * b[j];
    }

    MPI_Reduce(ab, result, m, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    elapsed_time += MPI_Wtime();

    if (!id) {
        printf("\n");
        fflush(stdout);
        for (i = 0; i < m; i++) {
            printf("%lf ", result[i]);
            fflush(stdout);
        }
        printf("\nRun Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }

    free(begin);
    free(end);
    free(num);
    free(a_row);
    free(b_vector);
    free(result);
    free(a);
    free(b);
    free(ab);

    MPI_Finalize();
    return 0;
}