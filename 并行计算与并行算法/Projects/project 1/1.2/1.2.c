/*
 * Author: Swhua
 */

#include <cfloat>
#include <ctime>
#include <math.h>
#include <mpi.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// 无需声明额外一维指针，也无需返回二级指针，但必须传三级指针！
void malloc_matrix(double ***matrix, int m, int n) {
    *matrix = (double **)malloc(m * sizeof(double *));
    *matrix[0] = (double *)malloc(m * n * sizeof(double));
    for (int i = 1; i < m; i++)
        // (*matrix)[i] = &((*matrix)[0][i * n]); // 两种都行
        (*matrix)[i] = (*matrix)[0] + i * n;
}

// 这里就不需要传三级指针！
void init_matrix(double **matrix, int m, int n) {
    // 不加static，两个矩阵会生成同样的数
    static default_random_engine e(time(NULL));
    static uniform_real_distribution<double> u(-100, 100);
    int number = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = u(e); //++number;
}

void print_matrix(double **matrix, int m, int n, string info) {
    printf("%s", info.c_str());
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%lf ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void output_matrix(double **matrix, int m, int n, string filename) {
    FILE *f = fopen(filename.c_str(), "w");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (j)
                fputc(' ', f); // 每行末尾没空格
            fprintf(f, "%lf", matrix[i][j]);
        }
        fputc('\n', f);
    }
    fclose(f);
}

double **multiplication_matrix(double **a, double **b, int r, int rc, int c) {
    int i, j, q, id, p, rp,
        cp; // rp:单个进程分到的a的行数，cp：单个进程分到的b的行数
    int size[2];        // 一行一列有多少个格子
    int coord[2];       // 当前进程子网格的坐标
    int periods[2];     // MPI_Cart_create创建子通信域要用
    int *count, *begin; // 每个进程（格子）处理的行数或列数、偏移
    double **ap, **bp, **abp; // 单个进程的数据与结果
    double **ab_row, **ab;    // ab_row：一行格子的乘积结果
    MPI_Comm cart_comm, row_comm, col_comm;
    MPI_Status status;
    MPI_Request pending_a, pending_b;
    size[0] = size[1] = 0;
    periods[0] = periods[1] = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Dims_create(p, 2, size);
    if (!id) {
        printf("size[0]=%d, size[1]=%d\n", size[0], size[1]);
        fflush(stdout);
    }
    MPI_Cart_create(MPI_COMM_WORLD, 2, size, periods, 0, &cart_comm);
    MPI_Cart_coords(cart_comm, id, 2, coord);
    // printf("id=%d, (%d,%d)\n", id, coord[0], coord[1]);
    // fflush(stdout);
    MPI_Comm_split(cart_comm, coord[0], coord[1],
                   &row_comm); // 把同一行的格子放在同一个通信域里
    MPI_Comm_split(cart_comm, coord[1], coord[0], &col_comm);
    rp = (coord[0] + 1) * r / size[0] - coord[0] * r / size[0]; // 格子里的行数
    cp = (coord[1] + 1) * c / size[1] - coord[1] * c / size[1]; // 格子里的列数
    malloc_matrix(&ap, rp, rc); // 当前进程分到a矩阵的部分空间
    malloc_matrix(&bp, cp, rc);
    malloc_matrix(&abp, rp, cp);
    memset(abp[0], 0, rp * cp * sizeof(double)); // bug: 少了[0]
    // printf("id=%d, rp=%d, cp=%d.\n", id, rp, cp);
    // fflush(stdout);
    // 分发数据
    if (!id) {
        //遍历每个网格
        int grid_coord[2], grid_id, a_row_no, a_row_num, b_col_no,
            b_col_num; // 每个网格在ab中的起始行号列号和行数列数
        for (grid_coord[0] = 0; grid_coord[0] < size[0]; grid_coord[0]++) {
            for (grid_coord[1] = 0; grid_coord[1] < size[1]; grid_coord[1]++) {
                if (grid_coord[0] == 0 & grid_coord[1] == 0)
                    continue; // 自己的网格数据不需要发送
                a_row_no = grid_coord[0] * r / size[0];
                a_row_num = (grid_coord[0] + 1) * r / size[0] - a_row_no;
                b_col_no = grid_coord[1] * c / size[1];
                b_col_num = (grid_coord[1] + 1) * c / size[1] - b_col_no;
                MPI_Cart_rank(
                    cart_comm, grid_coord,
                    &grid_id); // cart_comm中grid_coord坐标对应的网格id
                // printf("send a(%d,%d) b(%d,%d) to grid(%d,%d,%d)\n",
                // a_row_no,
                //        a_row_num, b_col_no, b_col_num, grid_coord[0],
                //        grid_coord[1], grid_id);
                // fflush(stdout);
                // 用非阻塞的慢些
                // MPI_Isend(a[a_row_no], a_row_num * rc, MPI_DOUBLE, grid_id,
                //           grid_id, MPI_COMM_WORLD, &pending_a);
                // MPI_Isend(b[b_col_no], b_col_num * rc, MPI_DOUBLE, grid_id,
                //           grid_id * 2, MPI_COMM_WORLD, &pending_b);
                MPI_Send(
                    a[a_row_no], a_row_num * rc, MPI_DOUBLE, grid_id, grid_id,
                    MPI_COMM_WORLD); // cart_comm和MPI_COMM_WORLD里id是一样的
                // printf("send to grid_id=%d\n", grid_id);
                MPI_Send(b[b_col_no], b_col_num * rc, MPI_DOUBLE, grid_id,
                         grid_id * 2, MPI_COMM_WORLD); // tag不一样
                // printf("send to grid_id=%d\n", grid_id);
            }
        }
        memcpy(ap[0], a[0], rp * rc * sizeof(double)); // 0号网格的数据
        memcpy(bp[0], b[0], cp * rc * sizeof(double));
    } else {
        MPI_Recv(ap[0], rp * rc, MPI_DOUBLE, 0, id, MPI_COMM_WORLD, &status);
        MPI_Recv(bp[0], cp * rc, MPI_DOUBLE, 0, id * 2, MPI_COMM_WORLD,
                 &status);
    }

    for (i = 0; i < rp; i++)
        for (j = 0; j < cp; j++)
            for (q = 0; q < rc; q++)
                abp[i][j] += ap[i][q] * bp[j][q]; // bp按列优先存储

    // 每行格子的结果收集到第一列来
    if (!coord[1]) {
        malloc_matrix(&ab_row, rp, c);
        // printf("id1=%d,ab_row_size=%d\n", id, rp * c);
        // fflush(stdout);
        count = (int *)malloc(size[1] * sizeof(int)); // 每个格子的列数
        begin =
            (int *)malloc(size[1] * sizeof(int)); // 偏移，每个格子的起始列号
        for (i = 0; i < size[1]; i++) {
            begin[i] = i * c / size[1];
            count[i] = (i + 1) * c / size[1] - begin[i];
        }
        // printf("id = %d, count[0]=%d, begin[0]=%d.\n", id, count[0],
        // begin[0]); fflush(stdout);
    }
    // printf("id2=%d, size=%d\n", id, rp * cp);
    // fflush(stdout);
    // 每行格子里也必须一行一行的传
    for (i = 0; i < rp; i++) {
        if (!coord[1])
            MPI_Gatherv(abp[i], cp, MPI_DOUBLE, ab_row[i], count, begin,
                        MPI_DOUBLE, 0, row_comm);
        else // 没有分配ab_row[i], count, begin三个数组
            MPI_Gatherv(abp[i], cp, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0,
                        row_comm);
    }

    // 第一列每行的结果收集到第一行来
    if (!coord[1]) {
        if (!coord[0]) {
            malloc_matrix(&ab, r, c);
            free(count);
            free(begin);
            count = (int *)malloc(size[0] * sizeof(int));
            begin = (int *)malloc(size[0] * sizeof(int));
            for (i = 0; i < size[0]; i++) {
                begin[i] = i * r / size[0] * c;
                count[i] = (i + 1) * r / size[0] * c - begin[i];
            }
            // for (i = 0; i < size[0]; i++) {
            //     printf("begin%d = %d, count%d = %d\n", i, begin[i], i,
            //            count[i]);
            //     fflush(stdout);
            // }
        }
        // for (i = 0; i < rp; i++) {
        //     for (j = 0; j < c; j++) {
        //         printf("%lf ", ab_row[i][j]);
        //         fflush(stdout);
        //     }
        //     printf(", id=%d\n", id);
        //     fflush(stdout);
        // }
        if (!coord[0])
            MPI_Gatherv(ab_row[0], rp * c, MPI_DOUBLE, ab[0], count, begin,
                        MPI_DOUBLE, 0, col_comm);
        else
            MPI_Gatherv(ab_row[0], rp * c, MPI_DOUBLE, NULL, NULL, NULL,
                        MPI_DOUBLE, 0, col_comm);
    }

    if (!id) {
        // printf("Multiplication result:\n");
        // fflush(stdout);
        // for (i = 0; i < r; i++) {
        //     for (j = 0; j < c; j++) {
        //         printf("%lf ", ab[i][j]);
        //         fflush(stdout);
        //     }
        //     printf("\n");
        //     fflush(stdout);
        // }
        // printf("\n");
        // fflush(stdout);
        return ab;
    } else
        return NULL;
}

double **convolution_matrix(double **a, double **kernel, int r, int c, int k) {
    int i, j, i2, j2, id, p, rp,
        cp; // rp:单个进程分到的a的行数，cp：单个进程分到的a的列数
    int size[2];        // 一行一列有多少个格子
    int coord[2];       // 当前进程子网格的坐标
    int periods[2];     // MPI_Cart_create创建子通信域要用
    int *count, *begin; // 每个进程（格子）处理的行数或列数、偏移
    double **ap, **kp, **akp; // 单个进程的数据与结果
    double **ak_row, **ak;    // ak_row：一行格子的乘积结果
    MPI_Comm cart_comm, row_comm, col_comm;
    MPI_Status status;
    size[0] = size[1] = 0;
    periods[0] = periods[1] = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Dims_create(p, 2, size);
    // if (!id) {
    //     printf("size[0]=%d, size[1]=%d\n", size[0], size[1]);
    //     fflush(stdout);
    // }
    MPI_Cart_create(MPI_COMM_WORLD, 2, size, periods, 0, &cart_comm);
    MPI_Cart_coords(cart_comm, id, 2, coord);
    // printf("id=%d, (%d,%d)\n", id, coord[0], coord[1]);
    MPI_Comm_split(cart_comm, coord[0], coord[1],
                   &row_comm); // 把同一行的格子放在同一个通信域里
    MPI_Comm_split(cart_comm, coord[1], coord[0], &col_comm);

    r = r - k + 1;
    c = c - k + 1;
    rp = (coord[0] + 1) * r / size[0] - coord[0] * r / size[0]; // 格子里的行数
    cp = (coord[1] + 1) * c / size[1] - coord[1] * c / size[1]; // 格子里的列数
    malloc_matrix(&ap, rp + k - 1, cp + k - 1); // 当前进程分到a矩阵的部分空间
    malloc_matrix(&kp, k, k);
    malloc_matrix(&akp, rp, cp);
    memset(akp[0], 0, rp * cp * sizeof(double));

    // 分发数据
    if (!id) {
        //遍历每个网格
        int grid_coord[2], grid_id, row_no, row_num, col_no,
            col_num; // 每个网格在ab中的起始行号列号和行数列数
        for (grid_coord[0] = 0; grid_coord[0] < size[0]; grid_coord[0]++) {
            for (grid_coord[1] = 0; grid_coord[1] < size[1]; grid_coord[1]++) {
                if (grid_coord[0] == 0 & grid_coord[1] == 0)
                    continue; // 自己的网格数据不需要发送
                row_no = grid_coord[0] * r / size[0];
                row_num = (grid_coord[0] + 1) * r / size[0] - row_no + k - 1;
                col_no = grid_coord[1] * c / size[1];
                col_num = (grid_coord[1] + 1) * c / size[1] - col_no + k - 1;
                MPI_Cart_rank(
                    cart_comm, grid_coord,
                    &grid_id); // cart_comm中grid_coord坐标对应的网格id
                // printf("send a(%d,%d,%d,%d)  to grid(%d,%d,%d)\n", row_no,
                //        row_num, col_no, col_num, grid_coord[0],
                //        grid_coord[1], grid_id);
                // fflush(stdout);
                for (i = row_no; i < row_no + row_num; i++) {
                    MPI_Send(a[i] + col_no, col_num, MPI_DOUBLE, grid_id,
                             i - row_no, MPI_COMM_WORLD);
                }
                MPI_Send(
                    kernel[0], k * k, MPI_DOUBLE, grid_id, grid_id,
                    MPI_COMM_WORLD); // cart_comm和MPI_COMM_WORLD里id是一样的
            }
        }
        // bug:之前是kernel[i]，只导致第一行前两个数有错，后面的都对
        memcpy(kp[0], kernel[0], k * k * sizeof(double));

        for (i = 0; i < rp + k - 1; i++)
            memcpy(ap[i], a[i], (cp + k - 1) * sizeof(double)); // 0号网格的数据
    } else {
        for (i = 0; i < rp + k - 1; i++)
            MPI_Recv(ap[i], cp + k - 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD,
                     &status);
        MPI_Recv(kp[0], k * k, MPI_DOUBLE, 0, id, MPI_COMM_WORLD, &status);
    }

    for (i = 0; i < rp; i++)
        for (j = 0; j < cp; j++)
            for (i2 = 0; i2 < k; i2++)
                for (j2 = 0; j2 < k; j2++)
                    akp[i][j] += ap[i + i2][j + j2] * kp[i2][j2];

    // 每行格子的结果收集到第一列来
    if (!coord[1]) {
        malloc_matrix(&ak_row, rp, c);
        memset(ak_row[0], 0, rp * c * sizeof(double));
        count = (int *)malloc(size[1] * sizeof(int)); // 每个格子的列数
        begin =
            (int *)malloc(size[1] * sizeof(int)); // 偏移，每个格子的起始列号
        for (i = 0; i < size[1]; i++) {
            begin[i] = i * c / size[1];
            count[i] = (i + 1) * c / size[1] - begin[i];
        }
    }

    // 每行格子里也必须一行一行的传
    for (i = 0; i < rp; i++) {
        if (!coord[1])
            MPI_Gatherv(akp[i], cp, MPI_DOUBLE, ak_row[i], count, begin,
                        MPI_DOUBLE, 0, row_comm);
        else // 没有分配ab_row[i], count, begin三个数组
            MPI_Gatherv(akp[i], cp, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0,
                        row_comm);
    }

    // 第一列每行的结果收集到第一行来
    if (!coord[1]) {
        if (!coord[0]) {
            malloc_matrix(&ak, r, c);
            memset(ak[0], 0, r * c * sizeof(double));
            free(count);
            free(begin);
            count = (int *)malloc(size[0] * sizeof(int));
            begin = (int *)malloc(size[0] * sizeof(int));
            for (i = 0; i < size[0]; i++) {
                begin[i] = i * r / size[0] * c;
                count[i] = (i + 1) * r / size[0] * c - begin[i];
                // Bug: c不能放在size[0]左边
            }
            // for (i = 0; i < size[0]; i++) {
            //     printf("begin%d = %d, count%d = %d\n", i, begin[i], i,
            //            count[i]);
            //     fflush(stdout);
            // }
        }
        // for (i = 0; i < rp; i++) {
        //     for (j = 0; j < c; j++) {
        //         printf("%lf ", ak_row[i][j]);
        //         fflush(stdout);
        //     }
        //     printf(", id=%d\n", id);
        //     fflush(stdout);
        // }
        if (!coord[0])
            MPI_Gatherv(ak_row[0], rp * c, MPI_DOUBLE, ak[0], count, begin,
                        MPI_DOUBLE, 0, col_comm);
        else
            MPI_Gatherv(ak_row[0], rp * c, MPI_DOUBLE, NULL, NULL, NULL,
                        MPI_DOUBLE, 0, col_comm);
    }

    if (!id) {
        // printf("Convolution result:\n");
        // fflush(stdout);
        // for (i = 0; i < r; i++) {
        //     for (j = 0; j < c; j++) {
        //         printf("%lf ", ak[i][j]);
        //         fflush(stdout);
        //     }
        //     printf("\n");
        //     fflush(stdout);
        // }
        // printf("\n");
        // fflush(stdout);
        return ak;
    } else
        return NULL;
}

double **max_pooling_matrix(double **a, int r, int c, int k) {
    int i, j, i2, j2, id, p, rp,
        cp; // rp:单个进程分到的a的行数，cp：单个进程分到的a的列数
    int size[2];        // 一行一列有多少个格子
    int coord[2];       // 当前进程子网格的坐标
    int periods[2];     // MPI_Cart_create创建子通信域要用
    int *count, *begin; // 每个进程（格子）处理的行数或列数、偏移
    double **ap, **akp;    // 单个进程的数据与结果
    double **ak_row, **ak; // ak_row：一行格子的乘积结果
    MPI_Comm cart_comm, row_comm, col_comm;
    MPI_Status status;
    size[0] = size[1] = 0;
    periods[0] = periods[1] = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Dims_create(p, 2, size);
    // if (!id) {
    //     printf("size[0]=%d, size[1]=%d\n", size[0], size[1]);
    //     fflush(stdout);
    // }
    MPI_Cart_create(MPI_COMM_WORLD, 2, size, periods, 0, &cart_comm);
    MPI_Cart_coords(cart_comm, id, 2, coord);
    // printf("id=%d, (%d,%d)\n", id, coord[0], coord[1]);
    MPI_Comm_split(cart_comm, coord[0], coord[1],
                   &row_comm); // 把同一行的格子放在同一个通信域里
    MPI_Comm_split(cart_comm, coord[1], coord[0], &col_comm);

    r = r - k + 1;
    c = c - k + 1;
    rp = (coord[0] + 1) * r / size[0] - coord[0] * r / size[0]; // 格子里的行数
    cp = (coord[1] + 1) * c / size[1] - coord[1] * c / size[1]; // 格子里的列数
    malloc_matrix(&ap, rp + k - 1, cp + k - 1); // 当前进程分到a矩阵的部分空间
    malloc_matrix(&akp, rp, cp);
    for (i = 0; i < rp; i++)
        for (j = 0; j < cp; j++)
            akp[i][j] = -DBL_MAX;
    // 分发数据
    if (!id) {
        //遍历每个网格
        int grid_coord[2], grid_id, row_no, row_num, col_no,
            col_num; // 每个网格在ab中的起始行号列号和行数列数
        for (grid_coord[0] = 0; grid_coord[0] < size[0]; grid_coord[0]++) {
            for (grid_coord[1] = 0; grid_coord[1] < size[1]; grid_coord[1]++) {
                if (grid_coord[0] == 0 & grid_coord[1] == 0)
                    continue; // 自己的网格数据不需要发送
                row_no = grid_coord[0] * r / size[0];
                row_num = (grid_coord[0] + 1) * r / size[0] - row_no + k - 1;
                col_no = grid_coord[1] * c / size[1];
                col_num = (grid_coord[1] + 1) * c / size[1] - col_no + k - 1;
                MPI_Cart_rank(
                    cart_comm, grid_coord,
                    &grid_id); // cart_comm中grid_coord坐标对应的网格id
                for (i = row_no; i < row_no + row_num; i++) {
                    MPI_Send(a[i] + col_no, col_num, MPI_DOUBLE, grid_id,
                             i - row_no, MPI_COMM_WORLD);
                }
            }
        }
        for (i = 0; i < rp + k - 1; i++)
            memcpy(ap[i], a[i], (cp + k - 1) * sizeof(double)); // 0号网格的数据
    } else {
        for (i = 0; i < rp + k - 1; i++)
            MPI_Recv(ap[i], cp + k - 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD,
                     &status);
    }

    for (i = 0; i < rp; i++)
        for (j = 0; j < cp; j++)
            for (i2 = 0; i2 < k; i2++)
                for (j2 = 0; j2 < k; j2++)
                    akp[i][j] =
                        max(akp[i][j], ap[i + i2][j + j2]); //* kp[i2][j2];

    // 每行格子的结果收集到第一列来
    if (!coord[1]) {
        malloc_matrix(&ak_row, rp, c);
        memset(ak_row[0], 0, rp * c * sizeof(double));
        count = (int *)malloc(size[1] * sizeof(int)); // 每个格子的列数
        begin = (int *)malloc(size[1] * sizeof(int));
        for (i = 0; i < size[1]; i++) {
            begin[i] = i * c / size[1];
            count[i] = (i + 1) * c / size[1] - begin[i];
        }
    }

    // 每行格子里也必须一行一行的传
    for (i = 0; i < rp; i++) {
        if (!coord[1])
            MPI_Gatherv(akp[i], cp, MPI_DOUBLE, ak_row[i], count, begin,
                        MPI_DOUBLE, 0, row_comm);
        else // 没有分配ab_row[i], count, begin三个数组
            MPI_Gatherv(akp[i], cp, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0,
                        row_comm);
    }

    // 第一列每行的结果收集到第一行来
    if (!coord[1]) {
        if (!coord[0]) {
            malloc_matrix(&ak, r, c);
            memset(ak[0], 0, r * c * sizeof(double));
            free(count);
            free(begin);
            count = (int *)malloc(size[0] * sizeof(int));
            begin = (int *)malloc(size[0] * sizeof(int));
            for (i = 0; i < size[0]; i++) {
                begin[i] = i * r / size[0] * c;
                count[i] = (i + 1) * r / size[0] * c - begin[i];
            }
        }

        if (!coord[0])
            MPI_Gatherv(ak_row[0], rp * c, MPI_DOUBLE, ak[0], count, begin,
                        MPI_DOUBLE, 0, col_comm);
        else
            MPI_Gatherv(ak_row[0], rp * c, MPI_DOUBLE, NULL, NULL, NULL,
                        MPI_DOUBLE, 0, col_comm);
    }

    if (!id) {
        // printf("max_pooling_result:\n");
        // fflush(stdout);
        // for (i = 0; i < r; i++) {
        //     for (j = 0; j < c; j++) {
        //         printf("%lf ", ak[i][j]);
        //         fflush(stdout);
        //     }
        //     printf("\n");
        //     fflush(stdout);
        // }
        // printf("\n");
        // fflush(stdout);
        return ak;
    } else
        return NULL;
}

// mpiexec -n 1 -f config ./1.2 4 3 5 2
int main(int argc, char **argv) {
    int p, id, r, rc, c, k, i, j;
    double elapsed_time;
    double **a, **b, **kernel; // double a[r][rc],b[rc][c],kernel[k][k];
    double **ab, **ak, **ap;
    if (argc != 5) {
        if (!id)
            printf("%s misses parameter 'n'!\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    r = atoi(argv[1]);  // 第一个矩阵a的行
    rc = atoi(argv[2]); // 第一个矩阵a的列，第二个矩阵b的行
    c = atoi(argv[3]);  // 第二个矩阵b的列
    k = atoi(argv[4]);  // kernel size

    // 计算矩阵乘法
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    if (!id) {
        malloc_matrix(&a, r, rc);
        malloc_matrix(&b, c, rc); // 直接按列存储b，求a*b^T
        init_matrix(a, r, rc);
        init_matrix(b, c, rc);
        // print_matrix(a, r, rc, "a1=\n");
        // print_matrix(b, c, rc, "b=\n");
        output_matrix(a, r, rc, "a1.txt");
        output_matrix(b, c, rc, "b^T.txt"); // bug: r,rc->c,rc // b的转置
    }
    ab = multiplication_matrix(a, b, r, rc, c);
    if (!id)
        output_matrix(ab, r, c, "ab.txt");
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("\nab run Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }

    // 计算Convolution
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    if (!id) {
        malloc_matrix(&kernel, k, k);
        init_matrix(a, r, rc);
        init_matrix(kernel, k, k);
        // print_matrix(a, r, rc, "a2=\n");
        // print_matrix(kernel, k, k, "kernel=\n");
        output_matrix(a, r, rc, "a2.txt");
        output_matrix(kernel, k, k, "k.txt");
    }
    ak = convolution_matrix(a, kernel, r, rc, k);
    if (!id)
        output_matrix(ak, r - k + 1, rc - k + 1, "ak.txt");
    // bug: 第一个参数ak抄成了ab
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("\nak run Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }

    // 计算max_pooling
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    if (!id) {
        init_matrix(a, r, rc);
        // print_matrix(a, r, rc, "a3=\n");
        output_matrix(a, r, rc, "a3.txt");
    }
    ap = max_pooling_matrix(a, r, rc, k);
    if (!id)
        output_matrix(ap, r - k + 1, rc - k + 1, "a_max_pooling.txt");
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("\na_max_pooling run Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }

    free(a);
    free(b);
    free(kernel);
    free(ab);
    free(ak);
    free(ap);
    MPI_Finalize();
}
