#include "bf-omp.h"
#include <algorithm>
#include "util.h"

#include <iostream>

using namespace std;

void bf_single(int p, int n, int *mat, int *dist, bool *has_negative_cycle) {
    (void) p;
    dist[0] = 0;
    for (int i = 1; i < n; ++i)
        dist[i] = INF;
    bool has_change = false;

    bool * last_round = new bool[n];
    fill_n(last_round, n, false);
    last_round[0] = true;

    bool * this_round = new bool[n];
    fill_n(this_round, n, false);

    int *times = new int[n];
    fill_n(times, n, 0);

    int my_load = n;
    int my_begin = 0;
    int my_end = my_begin + my_load;
    while (true) {
        // 1 round
        has_change = false;
        for (int u = 0; u < n; u++) {
            if (last_round[u]) {
                for (int v = my_begin; v < my_end; ++v) {
                    int weight = mat[u * n + v];
                    if (weight < INF)
                        if (dist[u] + weight < dist[v]) {
                            dist[v] = dist[u] + weight;
                            times[v] += 1;
                            this_round[v] = true;
                            has_change = true;
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
        if (!has_change) {
            goto END;
        }
        if (*has_negative_cycle) {
            goto END;
        }
        swap(last_round, this_round);
        fill_n(this_round, n, false);
    }
    END : {}


    delete[] last_round;
    delete[] this_round;
    delete[] times;
}

