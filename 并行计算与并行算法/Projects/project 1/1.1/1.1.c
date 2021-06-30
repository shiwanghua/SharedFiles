/*
 * Author: Swhua
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>

int Swhua_Bcast(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                int root, MPI_Comm comm) {
    int id, p, from, base, to;
    MPI_Request pending;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // 1->2->3->5->09
    //    2->4->6->10
    //       3->7->11
    //       4->8->12
    //          5->13
    //          6->14
    //          7->15
    //          8->16

    // // root ==0 版本
    // if (id) // 需要接收数据
    // {
    //     from = id - (int)pow(2.0, (double)(int)(log(id) / log(2)));
    //     MPI_Recv(data, count, datatype, from, id, comm, MPI_STATUS_IGNORE);
    //     base = pow(2.0, (double)(int)(log(id) / log(2)) + 1);
    // } else
    //     base = 1;
    // to = id + base;
    // while (to < p) {
    //     MPI_Send(data, count, datatype, to, to, comm);
    //     base = base << 1;
    //     to = id + base;
    // }

    // root版本: 把root映射成0，把0映射成root，0是虚拟源
    if (id != root) // 需要接收数据
    {
        if (id != 0) {
            from = id - (int)pow(2.0, (double)(int)(log(id) / log(2)));
            if (from == 0)
                from = root;
            else if (from == root)
                from = 0;
        } else { // id==0
            from = root - (int)pow(2.0, (double)(int)(log(root) / log(2)));
            if (from == 0)
                from = root;
        }
        // printf("p%d needs data from p%d\n", id, from);
        // fflush(stdout);

        MPI_Recv((double *)recvbuf + count * root, count, datatype, from, id,
                 comm, MPI_STATUS_IGNORE);

        if (id != 0) {
            base = pow(2.0, (double)(int)(log(id) / log(2)) + 1);
            to = id + base;
        } else {
            base = pow(2.0, (double)(int)(log(root) / log(2)) + 1);
            to = root + base;
        }
    } else {
        base = 1;
        to = 0 + base;
        int i = count * root, j = 0;
        for (; j < count; i++, j++)
            *((double *)recvbuf + i) = *((double *)sendbuf + j);
    }

    while (to < p) {
        if (to == root)
            to = 0;
        // printf("p%d sends data to p%d\n", id, to);
        // fflush(stdout);
        // MPI_Isend((double *)recvbuf + count * id, count, datatype, to, to,
        // comm,
        //           &pending); // 慢些！
        MPI_Send((double *)recvbuf + count * root, count, datatype, to, to,
                 comm);
        base = base << 1;
        if (id == root)
            to = 0 + base;
        else if (id == 0)
            to = root + base;
        else
            to = id + base;
    }

    return 0;
}

int Swhua_Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                    void *recvbuf, int recvcount, MPI_Datatype recvtype,
                    MPI_Comm comm) {
    int id, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    for (int root = 0; root < p; root++)
        Swhua_Bcast(sendbuf, recvbuf, sendcount, sendtype, root, comm);
    return 0;
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));
    int p, id, n, i, root;
    double *send_data, *recv_data, size, elapsed_time_Swhua, elapsed_time_MPI;

    if (argc != 2) {
        if (!id)
            printf("%s misses parameters!\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    n = atoi(argv[1]); // 数的个数
    size = sizeof(double) * n * p / 1024.0 / 1024.0;
    send_data = new double[n];
    // for (i = 0; i < n; i++)
    //     send_data[i] = id * i;
    recv_data = new double[n * p];
    if (send_data == NULL || recv_data == NULL) {
        printf("Cannot allocate enough memory: size=%d\n", n);
        MPI_Finalize();
        exit(1);
    }
    if (!id) {
        printf("size: %3.6f MB\np=%d\n", size, p);
        fflush(stdout);
    }

    // Bcast
    for (i = 0; i < n; i++)
        send_data[i] = rand() / 1000.0;
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time_MPI = -MPI_Wtime();
    MPI_Allgather(send_data, n, MPI_DOUBLE, recv_data, n, MPI_DOUBLE,
                  MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time_MPI += MPI_Wtime();
    if (!id) {
        printf("MPI_Allgather Time: %5.9f s\n\n", elapsed_time_MPI);
        fflush(stdout);
        // for (i = 0; i < p; i++) {
        //     for (int j = 0; j < n; j++) {
        //         printf("%2.1f ", recv_data[i * n + j]);
        //         fflush(stdout);
        //     }
        //     printf("\n");
        //     fflush(stdout);
        // }
        // printf("\n");
        // fflush(stdout);
    }

    // MyCast
    for (i = 0; i < n; i++)
        send_data[i] = rand() / 1000.0;
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time_Swhua = -MPI_Wtime();
    Swhua_Allgather(send_data, n, MPI_DOUBLE, recv_data, n, MPI_DOUBLE,
                    MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time_Swhua += MPI_Wtime();
    if (!id) {
        printf("Swhua_Allgather Time: %5.9f s\n", elapsed_time_Swhua);
        fflush(stdout);
        // for (i = 0; i < p; i++) {
        //     for (int j = 0; j < n; j++) {
        //         printf("%2.3f ", recv_data[i * n + j]);
        //         fflush(stdout);
        //     }
        //     printf("\n");
        //     fflush(stdout);
        // }
    }

    delete send_data, recv_data;
    MPI_Finalize();
}
