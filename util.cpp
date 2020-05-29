#include "util.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

int N;
int *mat;

void _die(string msg) {
    cerr << msg << endl;
    exit(1);
}

void read_matrix(string fname) {
    ifstream f(fname, ifstream::in);
    if (!f.good()) {
        _die("Failed read from file");
    }
    f >> N;
    mat = (int *) malloc(N * N * sizeof(int));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            f >> mat[i * N + j];
        }
    f.close();
}

int print_result(bool has_negative_cycle, int *dist, const char *outputFileName) {
    ofstream f(outputFileName, ofstream::out);
    if (!has_negative_cycle) {
        for (int i = 0; i < N; i++) {
            if (dist[i] > INF)
                dist[i] = INF;
            f << dist[i] << '\n';
        }
    } else {
        f << "Negative cycle was found =(" << endl;
    }
    f.close();
    return 0;
}


