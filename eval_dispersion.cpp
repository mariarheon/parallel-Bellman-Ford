#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <sys/time.h>
#include <math.h>

#include "util.h"
#include "bf-omp.h"
#include "bf-pthread.h"
#include "bf-single.h"

using namespace std;

typedef void (*BFFunc)(int p, int n, int *mat, int *dist, bool *has_negative_cycle);

float algo(BFFunc bfFunc, int p, const char *outputFileName) {
	(void)outputFileName;
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
    // cerr << setprecision(6) << "Time(s): " << (ms_wall/1000.0) << " for " << outputFileName << endl;
    // print_result(has_negative_cycle, dist, outputFileName);
    delete[] dist;
	return ms_wall;
}

void print_res(const char *what, double avg, double d, double maxError) {
	cout << what << ": m = " << avg << " ms, d = " << d << endl;
	cout << "99% interval: " << avg << " +- " << maxError << " ms" << endl;
	cout << endl;
}

int main(int argc, char **argv) {
    if (argc <= 3) {
        _die("<prog> <mat_file_name> <thread_count> <output_file_prefix>");
    }
    string fname = argv[1];
    int p = atoi(argv[2]);
    string output_file_prefix = argv[3];

    read_matrix(fname);

	const int TIMES = 50;
	double tSingle[TIMES];
	double tOmp[TIMES];
	double tPthread[TIMES];
	double avgSingle = 0;
	double avgOmp = 0;
	double avgPthread = 0;
	double dSingle = 0;
	double dOmp = 0;
	double dPthread = 0;
	for (int i = 0; i < TIMES; i++) {
    	tSingle[i] = algo(bf_single, p, (output_file_prefix + "_single.txt").c_str());
	    tOmp[i] = algo(bf_omp, p, (output_file_prefix + "_omp.txt").c_str());
    	tPthread[i] = algo(bf_pthread, p, (output_file_prefix + "_pthread.txt").c_str());
		avgSingle += tSingle[i];
		avgOmp += tOmp[i];
		avgPthread += tPthread[i];
	}
	avgSingle /= TIMES;
	avgOmp /= TIMES;
	avgPthread /= TIMES;
	for (int i = 0; i < TIMES; i++) {
		dSingle += (avgSingle - tSingle[i])*(avgSingle - tSingle[i]);
		dOmp += (avgOmp - tOmp[i]) * (avgOmp - tOmp[i]);
		dPthread += (avgPthread - tPthread[i]) * (avgPthread - tPthread[i]);
	}
	dSingle /= TIMES - 1;
	dOmp /= TIMES - 1;
	dPthread /= TIMES - 1;
	double maxErrorSingle = 2.6778 * sqrt(dSingle / TIMES);
	double maxErrorOmp = 2.6778 * sqrt(dOmp / TIMES);
	double maxErrorPthread = 2.6778 * sqrt(dPthread / TIMES);
	print_res("Single", avgSingle, dSingle, maxErrorSingle);
	print_res("Omp", avgOmp, dOmp, maxErrorOmp);
	print_res("Pthread", avgPthread, dPthread, maxErrorPthread);
    delete[] mat;

    return 0;
}
