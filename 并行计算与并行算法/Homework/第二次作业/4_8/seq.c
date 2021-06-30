#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define N 1000001
bool vis[N];
int primes[N];

int _end = 0;
int c1 = 0, c2 = 0;
void get_primes(int n) {
    for (int i = 2; i <= n; ++i) {
        if (!vis[i])
            primes[_end++] = i;
        for (int j = 0; primes[j] <= n / i; ++j) {
            vis[primes[j] * i] = true;
            //如果是倍数，前面赋值过了
            if (i % primes[j] == 0)
                break;
        }
    }
}

bool is(int n) {
    for (int i = 2; i <= (int)sqrt(n); ++i)
        if (n % i == 0)
            return false;
    return true;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    double elapsed_time = -MPI_Wtime();
    get_primes(1000000);
    for (int i = 0; i < _end - 1; i++)
        if (primes[i] + 2 == primes[i + 1])
            c1++;
    bool last = 0;
    for (int i = 3; i <= 1000000; i += 2)
        if (is(i)) {
            if (last)
                c2++;
            else
                last = 1;
        } else
            last = 0;

    elapsed_time += MPI_Wtime();
    printf("%d\nc1=%d\nc2=%d\nTime=%5.9fs.", _end, c1, c2, elapsed_time);
    MPI_Finalize();
    return 0;
}

// 78498
// 8169
// 0.01694s