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

void algo(BFFunc bfFunc, int p, const char *algo_type) {
    int *dist;
    bool has_negative_cycle = false;
    dist = new int[N];

    bfFunc(p, N, mat, dist, &has_negative_cycle);

	int expected[] = {0, -1, 2, -2, 1};
	bool testFailed = false;
	for (int i = 0; i < 5; i++) {
		if (dist[i] != expected[i]) {
			testFailed = true;
			break;
		}
	}
	if (testFailed) {
		cerr << "Test failed";
	} else {
		cerr << "Test successfully passed";
	}
	cerr << " for " << algo_type << " type of algorithm" << endl;
    delete[] dist;
}

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;
    string fname = "my_mat";
    int p = 4;

    read_matrix(fname);
    algo(bf_single, p, "single");
    algo(bf_omp, p, "omp");
    algo(bf_pthread, p, "pthread");
    delete[] mat;

    return 0;
}
