#ifndef UTIL_H
#define UTIL_H

#include <string>

using namespace std;

#define INF 1000000

void _die(string msg);

void read_matrix(string fname);

int print_result(bool has_negative_cycle, int *dist, const char *outputFileName);

extern int N;
extern int *mat;

#endif
