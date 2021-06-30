#include <cfloat>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unordered_set>
#include <vector>

using namespace std;

#define DampingCoefficient 0.85
#define epoch 100 // 迭代多少轮

int main(int argc, char **argv) {
    int n = 1024000;     // page页数
    int maxLinkNum = 10; // 单页的最大链接数
    int p = 10;          // 并行线程数
    double elapsed_time;
    // n = (int)strtod(argv[1], NULL);
    // cout << n << endl;
    n = atoi(argv[1]);
    maxLinkNum = atoi(argv[2]);
    p = atoi(argv[3]);

    vector<unordered_set<int>> pageLink(n);
    double score[n];
    double *tempSum = (double *)malloc(sizeof(double) * n);
    default_random_engine e(time(NULL));
    uniform_int_distribution<int> linkNumGenerator(1,
                                                   maxLinkNum); // 生成链接个数
    uniform_int_distribution<int> linkPageGenerator(0, n - 1); // 生成链接页号

    // elapsed_time = -omp_get_wtime();
    // for (int i = 0; i < n; i++) {
    //    int num = linkNumGenerator(e), pageNo;
    //    while (pageLink[i].size() < num) {
    //        pageNo = linkPageGenerator(e);
    //        pageLink[i].insert(pageNo);
    //    }
    //    score[i] = 1.0;
    //}
    // elapsed_time += omp_get_wtime();
    // printf("NoOMP InitializationTime: %lf s\n", elapsed_time);

    // elapsed_time = -omp_get_wtime();
    // for (int i = 0; i < epoch; i++) {
    //    memset(tempSum, 0, sizeof(tempSum));
    //    for (int j = 0; j < n; j++) {
    //        // outScore: 当前页可输出给每个链接的分数
    //        double outScore = score[j] / pageLink[j].size();
    //        unordered_set<int>::iterator iter = pageLink[j].begin();
    //        while (iter != pageLink[j].end()) {
    //            //#pragma omp atomic
    //            tempSum[*iter] = tempSum[*iter] + outScore;
    //            iter++;
    //        }
    //    }
    //    for (int j = 0; j < n; j++) {
    //        score[j] = tempSum[j] * DampingCoefficient + 1 -
    //        DampingCoefficient;
    //    }
    //}
    // elapsed_time += omp_get_wtime();
    // for (int i = (int)(n * DampingCoefficient);
    //    i < min(n, (int)(n * DampingCoefficient) + 10); i++)
    //    printf("pageScore[%d]=%lf\n", i, score[i]);
    // printf("n=%d, NOOMP RunningTime= %5.6f s\n", n, elapsed_time);

    // for (int i = 0; i < n; i++) {
    //    pageLink[i].clear();
    //}

    omp_set_num_threads(p);

    elapsed_time = -omp_get_wtime();
    // #pragma omp parallel for shared(n, pageLink, score)
    for (int i = 0; i < n; i++) {
        int num = linkNumGenerator(e), pageNo;
        while (pageLink[i].size() < num) {
            pageNo = linkPageGenerator(e);
            pageLink[i].insert(pageNo);
        }
        score[i] = 1.0;
    }
    elapsed_time += omp_get_wtime();
    // printf("OMP InitializationTime: %lf s\n", elapsed_time);

    elapsed_time = -omp_get_wtime();
    for (int i = 0; i < epoch; i++) {
        memset(tempSum, 0, sizeof(tempSum));
#pragma omp parallel for shared(n, pageLink, tempSum)                          \
    schedule(guided) //不能nowait   比static稍微快一点点
        for (int j = 0; j < n; j++) {
            // outScore: 当前页可输出给每个链接的分数
            double outScore = score[j] / pageLink[j].size();
            unordered_set<int>::iterator iter = pageLink[j].begin();
            while (iter != pageLink[j].end()) {
                //#pragma omp atomic
                tempSum[*iter] = tempSum[*iter] + outScore;
                iter++;
            }
        }
#pragma omp parallel for shared(n, tempSum, score)                             \
    schedule(guided) // runtime guided dynamic static
        for (int j = 0; j < n; j++) {
            score[j] = tempSum[j] * DampingCoefficient / maxLinkNum + 1 -
                       DampingCoefficient;
        }

        // 看分数的变化情况
        // for (int i = (int)(n * DampingCoefficient);
        //      i < min(n, (int)(n * DampingCoefficient) + 10); i++)
        //     printf("pageScore[%d]=%lf\n", i, score[i]);
    }
    elapsed_time += omp_get_wtime();
    // for (int i = (int)(n * DampingCoefficient);
    //      i < min(n, (int)(n * DampingCoefficient) + 10); i++)
    //     printf("pageScore[%d]=%lf\n", i, score[i]);
    printf("n=%d, p=%d, OMP RunningTime= %5.6f s\n", n, p, elapsed_time);
    fflush(stdout);

    free(tempSum);
    return 0;
}
