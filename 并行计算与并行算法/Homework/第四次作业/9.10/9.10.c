#include <float.h>
#include <math.h>
#include <mpi.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <time.h>
using namespace std;

#define MANAGER_ID 0
#define Perfect_MSG 0
#define n_MSG 1
#define EMPTY_MSG 2
typedef signed long long int ll;

bool isPrime(ll n) {
    for (ll i = 2; i <= (ll)sqrt((ll)n); ++i)
        if (n % i == 0)
            return false;
    return true;
}

// 求前8个perfect number（所有除了自己以外的正因子的和等于本身）
// 如果2^n-1是质数，则(2^n-1)*2^(n-1)是perfect number
// mpiexec -n 4 -f config ./9.10
int main(int argc, char *argv[]) {

    int id, p;
    double elapsed_time;
    void manager(int, char **, int);
    void worker();
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    if (p < 2) {
        printf("至少要两个进程\n");
        fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, 2);
        exit(2);
    } else if (!id)
        manager(argc, argv, p);
    else
        worker();

    elapsed_time += MPI_Wtime();
    if (id == MANAGER_ID) {
        printf("\nRun Time: %3.6f seconds\n", elapsed_time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}

void manager(int argc, char *argv[], int p) {
    int i, N, id, source, msgTag, get = 0, numTerminated = 0;
    int *numP = (int *)malloc(p * sizeof(int));
    memset(numP, 0, p * sizeof(int));
    ll n, buffer;
    MPI_Request pending;
    MPI_Status status;
    N = 8;
    n = 2; // 第一个质数
    ll *perfect = (ll *)malloc(N * sizeof(ll));
    do {
        MPI_Recv(&buffer, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        source = status.MPI_SOURCE;
        msgTag = status.MPI_TAG;
        if (msgTag == Perfect_MSG)
            perfect[get++] = buffer;
        if (get < N && n < 32) {
            MPI_Send(&n, 1, MPI_LONG_LONG, source, n_MSG, MPI_COMM_WORLD);
            numP[source]++;
            n++;
        } else {
            MPI_Send(NULL, 0, MPI_LONG_LONG, source, EMPTY_MSG, MPI_COMM_WORLD);
            numTerminated++;
        }
    } while (numTerminated < p - 1);

    printf("\n%d 个Perfect Number: ", N);
    fflush(stdout);
    for (i = 0; i < N - 1; i++) {
        printf("%lld, ", perfect[i]);
        fflush(stdout);
    }
    printf("%lld.\n\n", perfect[N - 1]);
    fflush(stdout);

    for (i = 0; i < p; i++) {
        printf("p%d: %d\n", i, numP[i]);
        fflush(stdout);
    }
    free(perfect);
    free(numP);
}

void worker() {
    int msgLen, id;
    MPI_Request pending;
    MPI_Status status;
    ll n;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Isend(NULL, 0, MPI_LONG_LONG, MANAGER_ID, EMPTY_MSG, MPI_COMM_WORLD,
              &pending);
    while (true) {
        MPI_Probe(MANAGER_ID, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_LONG_LONG, &msgLen);
        if (!msgLen)
            break;
        MPI_Recv(&n, 1, MPI_LONG_LONG, MANAGER_ID, n_MSG, MPI_COMM_WORLD,
                 &status);
        if (isPrime((ll)(1 << n) - 1)) {
            n = (ll)((1 << n) - 1) * (ll)(1 << (n - 1));
            MPI_Send(&n, 1, MPI_LONG_LONG, MANAGER_ID, Perfect_MSG,
                     MPI_COMM_WORLD);
            // printf("p%d find a perfect number: %lld\n", id, n);
            // fflush(stdout);
        } else
            MPI_Send(NULL, 0, MPI_LONG_LONG, MANAGER_ID, EMPTY_MSG,
                     MPI_COMM_WORLD);
    }
}