#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <sys/time.h>

#include "util.h"
#include "bf-omp.h"
#include "bf-pthread.h"
#include "bf-single.h"

using namespace std;

typedef void (*BFFunc)(int p, int n, int *mat, int *dist, bool *has_negative_cycle);

void algo(BFFunc bfFunc, int p, const char *outputFileName) {
    int *dist;
    bool has_negative_cycle = false;
    dist = new int[N];

    timeval start_wall_time_t, end_wall_time_t;
    float ms_wall;

    gettimeofday(&start_wall_time_t, nullptr);

    bfFunc(p, N, mat, dist, &has_negative_cycle);

    gettimeofday(&end_wall_time_t, nullptr);
    ms_wall = ((end_wall_time_t.tv_sec - start_wall_time_t.tv_sec) * 1000 * 1000
               + end_wall_time_t.tv_usec - start_wall_time_t.tv_usec) / 1000.0;

    cerr.setf(ios::fixed);
    cerr << setprecision(6) << "Time(s): " << (ms_wall/1000.0) << " for " << outputFileName << endl;
    print_result(has_negative_cycle, dist, outputFileName);
    delete[] dist;
}

int main(int argc, char **argv) {
    if (argc <= 3) {
        _die("<prog> <mat_file_name> <thread_count> <output_file_prefix>");
    }
    string fname = argv[1];
    int p = atoi(argv[2]);
    string output_file_prefix = argv[3];

    read_matrix(fname);
    algo(bf_single, p, (output_file_prefix + "_single.txt").c_str());
    algo(bf_omp, p, (output_file_prefix + "_omp.txt").c_str());
    algo(bf_pthread, p, (output_file_prefix + "_pthread.txt").c_str());
    delete[] mat;

    return 0;
}
