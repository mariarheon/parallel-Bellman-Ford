#include "bf-omp.h"
#include "omp.h"
#include <algorithm>
#include "util.h"

#include <iostream>

using namespace std;

void bf_omp(int p, int n, int *mat, int *dist, bool *has_negative_cycle) {
    // allocation
    int q = n / p, r = n % p;
    int load[p], begin[p];
    load[0] = q;
    for (int i = 1; i < p; ++i)
        load[i] = q + ((i <= r) ? 1 : 0);
    begin[0] = 0;
    for (int i = 1; i < p; ++i)
        begin[i] = begin[i - 1] + load[i - 1];

    // initialization
    dist[0] = 0;
    for (int i = 1; i < n; ++i)
        dist[i] = INF;
    bool has_change = false;
    *has_negative_cycle = false;

    bool *last_round = new bool[n];
    fill_n(last_round, n, false);
    last_round[0] = true;

    bool *this_round = new bool[n];
    fill_n(this_round, n, false);

    int *times = new int[n];
    fill_n(times, n, 0);
    #pragma omp parallel num_threads(p)
    {
        int my_rank = omp_get_thread_num();
        int my_load = load[my_rank];
        int my_begin = begin[my_rank];
        int my_end = my_begin + my_load;
        bool my_has_change = false;

        #pragma omp barrier

        for (size_t i = 0; ; ++i) {
            has_change = my_has_change = false;
            for (int u = 0; u < n; u++) {
                if (last_round[u]) {
                    for (int v = my_begin; v < my_end; ++v) {
                        int weight = mat[u * n + v];
                        if (weight < INF)
                            if (dist[u] + weight < dist[v]) {
                                #pragma omp critical
                                {
                                    dist[v] = dist[u] + weight;
                                    times[v] += 1;
                                }
                                this_round[v] = true;
                                my_has_change = true;
                                if (v == 0 && dist[v] < 0) {
                                    *has_negative_cycle = true;
                                }
                                if (times[v] == n) {
                                    *has_negative_cycle = true;
                                }
                            }
                    }
                }
            }
            #pragma omp barrier         
            #pragma omp critical
            {
                has_change = has_change || my_has_change;
            }
            #pragma omp barrier
            if (!has_change) {
                goto END;
            }
            if (*has_negative_cycle) {
                goto END;
            }
            #pragma omp barrier
            if (my_rank == 0) {
                swap(last_round, this_round);
                fill_n(this_round, n, false);
            }
            #pragma omp barrier
        }
        END : {}
    }

    delete[] last_round;
    delete[] this_round;
    delete[] times;
}

