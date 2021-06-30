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

double getTimeNs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)(ts.tv_sec * 1000000000 + ts.tv_nsec);
}

void quicksort_oneThread(double *&array, int len) {
    if (len <= 1)
        return;
    double pivot = array[len >> 1], temp;
    int left = 0, right = len - 1;
    while (left <= right) {
        while (array[left] < pivot)
            left++;
        while (array[right] > pivot)
            right--;
        if (left <= right) {
            // array[left] = array[left] | array[right];
            // array[right] = array[left] | array[right];
            // array[left] = array[left] | array[right];
            temp = array[right];
            array[right] = array[left];
            array[left] = temp;
            left++, right--;
        }
    }

    double *subArray[] = {array, &(array[left])};
    int subLen[] = {right + 1, len - left};
    quicksort_oneThread(subArray[0], subLen[0]);
    quicksort_oneThread(subArray[1], subLen[1]);
}

void quicksort_omp(double *&array, int len) {
    if (len <= 50) { // 插入排序
        double temp;
        for (int i = 1; i < len; i++) {
            int j = i;
            temp = array[i];
            while (j && temp < array[j - 1]) {
                array[j] = array[j - 1];
                j--;
            }
            array[j] = temp;
        }
        return;
    }
    double pivot = array[len >> 1], temp;
    int left = 0, right = len - 1;
    while (left <= right) {
        while (array[left] < pivot)
            left++;
        while (array[right] > pivot)
            right--;
        if (left <= right) {
            // array[left] = array[left] | array[right];
            // array[right] = array[left] | array[right];
            // array[left] = array[left] | array[right];
            temp = array[right];
            array[right] = array[left];
            array[left] = temp;
            left++, right--;
        }
    }

    double *subArray[] = {array, &(array[left])};
    int subLen[] = {right + 1, len - left};
#pragma omp task default(none) firstprivate(subArray, subLen)
    { quicksort_omp(subArray[0], subLen[0]); }
#pragma omp task default(none) firstprivate(subArray, subLen)
    { quicksort_omp(subArray[1], subLen[1]); }
}

void quicksort_omp2(double *&array, int len, int threadUsed) {
    if (len <= 50) {
        double temp;
        for (int i = 1; i < len; i++) {
            int j = i;
            temp = array[i];
            while (j && temp < array[j - 1]) {
                array[j] = array[j - 1];
                j--;
            }
            array[j] = temp;
        }
        return;
    }
    if (threadUsed > 16) {
        quicksort_oneThread(array, len);
        return;
    }
    double pivot = array[len >> 1], temp;
    int left = 0, right = len - 1;
    while (left <= right) {
        while (array[left] < pivot)
            left++;
        while (array[right] > pivot)
            right--;
        if (left <= right) {
            // array[left] = array[left] | array[right];
            // array[right] = array[left] | array[right];
            // array[left] = array[left] | array[right];
            temp = array[right];
            array[right] = array[left];
            array[left] = temp;
            left++, right--;
        }
    }

    double *subArray[] = {array, &(array[left])};
    int subLen[] = {right + 1, len - left};
#pragma omp task default(none) firstprivate(subArray, subLen, threadUsed)
    { quicksort_omp2(subArray[0], subLen[0], threadUsed << 1); }
#pragma omp task default(none) firstprivate(subArray, subLen, threadUsed)
    { quicksort_omp2(subArray[1], subLen[1], threadUsed << 1); }
}

bool checkSorted(double array[], int len) {
    for (int i = 1; i < len; i++)
        if (array[i] < array[i - 1])
            return false;
    return true;
}

int main(int argc, char **argv) {
    int i, n = 1000000;
    double elapsed_time, percentage = 0.7;
    // n = (int)strtod(argv[1], NULL);
    // cout << n << endl;
    n = atoi(argv[1]);
    const double maxValue = 10000000000;
    bool sorted;
    default_random_engine e(time(NULL));
    uniform_real_distribution<double> u(-maxValue, maxValue);
    double *array = (double *)malloc(n * sizeof(double));

    elapsed_time = -getTimeNs();
    for (i = 0; i < n; i++)
        array[i] = u(e);
    elapsed_time += getTimeNs();
    // printf("NoOMP InitializationTime: %lf s\n", elapsed_time / 1000000000);

    elapsed_time = -getTimeNs();
    quicksort_oneThread(array, n);
    elapsed_time += getTimeNs();
    // for (i = (int)(n * percentage); i < min(n, (int)(n * percentage) + 10);
    // i++)
    //     printf("array[%d]=%lf\n", i, array[i]);
    sorted = checkSorted(array, n);
    printf("n=%d, NoOMP RunningTime= %5.6f s, sorted=%d\n", n,
           elapsed_time / 1000000000, sorted);

    elapsed_time = -getTimeNs();
    // #pragma omp parallel num_threads(10)
    //     {
    //         default_random_engine e(time(NULL));
    //         uniform_real_distribution<double> u(0, maxValue);
    // #pragma omp parallel for shared(n) private(u, e)
    //         for (int i = 0; i < n; i++)
    //             array[i] = u(e);
    //     }
    for (i = 0; i < n; i++)
        array[i] = u(e);
    elapsed_time += getTimeNs();
    // printf("OMP InitializationTime: %lfs\n", elapsed_time / 1000000000);

    elapsed_time = -getTimeNs();
    quicksort_omp(array, n);
    // quicksort_omp2(array, n, 1);
    elapsed_time += getTimeNs();
    // for (i = (int)(n * percentage); i < min(n, (int)(n * percentage) + 10);
    // i++)
    //     printf("array[%d]=%lf\n", i, array[i]);
    sorted = checkSorted(array, n);
    printf("n=%d, OMP RunningTime= %5.6f s, sorted=%d\n", n,
           elapsed_time / 1000000000, sorted);
    fflush(stdout);
    return 0;
}
