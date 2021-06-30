#include <float.h>
#include <mpi.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

#define VECTOR_MSG 0
#define ROW_MSG 1     // 矩阵的1行数据或者k行数据
#define EMPTY_MSG 2   // manager发代表终止，worker发代表ready
#define Row_Res_MSG 3 // 一行乘列向量的结果
#define MANAGER_ID 0

// 用manager/worker-style实现矩阵按行乘向量
// manager负责读取和发送原数据
// mpiexec -n 2 -f ./config ./9.7
int main(int argc, char *argv[]) {
    int id, p;
    double elapsed_time;
    // 根据题意，worker进程间无需通信
    // 但也可以让worker进程去读向量，manager读矩阵
    // 这样就需要分通信域了
    // MPI_Comm worker_comm;
    void manager(int, char **, int);
    // void worker(MPI_Comm);
    void worker();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    if (argc != 3) {
        printf("输入参数太少！请输入m、n。\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    } else if (p < 2) {
        printf("至少要两个进程\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, 2);
        exit(2);
    } else {
        if (!id) {
            // MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, id, &worker_comm);
            manager(argc, argv, p);
        } else {
            // MPI_Comm_split(MPI_COMM_WORLD, 0, id, &worker_comm);
            // worker(argc, argv, worker_comm);
            worker();
        }
    }

    elapsed_time += MPI_Wtime();
    if (id == MANAGER_ID) {
        printf("\nRun Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}

void manager(int argc, char *argv[], int p) {
    int i, j, m, n;
    int source, msgTag, numTerminated = 0, assign_cnt = 0;
    int *assign_pid_rowid = (int *)malloc(p * sizeof(int)),
        *numP = (int *)malloc(p * sizeof(int));
    memset(assign_pid_rowid, 0, p * sizeof(int));
    memset(numP, 0, p * sizeof(int));
    double *a_row, *b, *c,
        buffer; // a_row[i]*b=c[i], buffer存储一行乘向量b的结果
    MPI_Request pending, pending_c;
    MPI_Status status;
    // FILE *f;

    // f = fopen("matrix_vector.txt", "r");
    // fscanf(f, "n=%d\n", &n);
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    MPI_Ibcast(&n, 1, MPI_INT, MANAGER_ID, MPI_COMM_WORLD, &pending);

    a_row = (double *)malloc(n * sizeof(double));
    b = (double *)malloc(n * sizeof(double));
    default_random_engine e(time(0));
    uniform_real_distribution<double> u(-1000, 1000);
    for (i = 0; i < n; i++) {
        // fscanf(f, "%lf ", &b[i]);
        b[i] = u(e);
    }
    MPI_Ibcast(b, n, MPI_DOUBLE, MANAGER_ID, MPI_COMM_WORLD, &pending);

    // fscanf(f, "m=%d\n", &m);
    c = (double *)malloc(m * sizeof(double));
    do {
        MPI_Irecv(&buffer, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, &pending_c);

        if (assign_cnt < m) {
            for (i = 0; i < n; i++) {
                // fscanf(f, "%lf", &a_row[i]);
                a_row[i] = u(e);
            }
        }
        MPI_Wait(&pending_c, &status);
        source = status.MPI_SOURCE;
        msgTag = status.MPI_TAG;
        if (msgTag == Row_Res_MSG) {
            c[assign_pid_rowid[source]] = buffer;
        }
        if (assign_cnt < m) {
            // printf("发第%d行数据给%d号进程\n", assign_cnt, source);
            // fflush(stdout);
            MPI_Send(a_row, n, MPI_DOUBLE, source, ROW_MSG, MPI_COMM_WORLD);
            assign_pid_rowid[source] = assign_cnt;
            numP[source]++;
            assign_cnt++;
        } else {
            MPI_Isend(NULL, 0, MPI_DOUBLE, source, EMPTY_MSG, MPI_COMM_WORLD,
                      &pending);
            numTerminated++;
        }

    } while (numTerminated < p - 1);

    // fclose(f);

    printf("\nResult c=ab=(");
    fflush(stdout);
    for (i = 0; i < m - 1; i++) {
        printf("%lf, ", c[i]);
        fflush(stdout);
    }
    printf("%lf)\n\n", c[m - 1]);
    fflush(stdout);

    for (i = 0; i < p; i++) {
        printf("p%d: %d\n", i, numP[i]);
        fflush(stdout);
    }
    free(assign_pid_rowid);
    free(a_row);
    free(b);
    free(c);
    free(numP);
}

void worker() {
    int i, n, msgLen, id;
    double *a_row, *b, c; // a^T*b=c[i]
    MPI_Request pending_n, pending_b, pending_c;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Isend(NULL, 0, MPI_DOUBLE, MANAGER_ID, EMPTY_MSG, MPI_COMM_WORLD,
              &pending_c);
    // MPI_Bcast(&n, 1, MPI_INT, MANAGER_ID, MPI_COMM_WORLD);
    MPI_Ibcast(&n, 1, MPI_INT, MANAGER_ID, MPI_COMM_WORLD, &pending_n);
    MPI_Wait(&pending_n, &status);
    b = (double *)malloc(n * sizeof(double));
    MPI_Ibcast(b, n, MPI_DOUBLE, MANAGER_ID, MPI_COMM_WORLD, &pending_b);
    a_row = (double *)malloc(n * sizeof(double));
    MPI_Wait(&pending_b, &status);
    while (true) {
        MPI_Probe(MANAGER_ID, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &msgLen);
        if (!msgLen)
            break;
        MPI_Recv(a_row, n, MPI_DOUBLE, MANAGER_ID, ROW_MSG, MPI_COMM_WORLD,
                 &status);
        c = 0.0;
        for (i = 0; i < n; i++)
            c += a_row[i] * b[i];
        MPI_Send(&c, 1, MPI_DOUBLE, MANAGER_ID, Row_Res_MSG, MPI_COMM_WORLD);
    }
    // printf("p%d is done.\n", id);
    // fflush(stdout);
    free(a_row);
    free(b);
}