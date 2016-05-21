#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <omp.h>

#define ROWS_NUM    4

using namespace std;

vector<double> func(int N, vector<double> matrix) {
	vector<double> alpha;
	vector<double> beta;
	vector<double> x;

	x.resize(N);

	alpha.push_back(-matrix[2] / matrix[1]);  // alpha_2 = -b_1 / c_1
	beta.push_back(matrix[3] / matrix[1]);    // beta_2  =  f_1 / c_1

	for (int i = 2; i < N; i++) {
		alpha.push_back(-matrix[ROWS_NUM * (i - 1) + 2]
			/ (matrix[ROWS_NUM * (i - 1)] * alpha[i - 2]
				+ matrix[ROWS_NUM * (i - 1) + 1]));  // alpha_i+1
		beta.push_back((matrix[ROWS_NUM * (i - 1) + 3]
			- matrix[ROWS_NUM * (i - 1)] * beta[i - 2])
			/ (matrix[ROWS_NUM * (i - 1)] * alpha[i - 2]
				+ matrix[ROWS_NUM * (i - 1) + 1]));  // beta_i+1
	}

	x[N - 1] = (matrix[ROWS_NUM * (N - 1) + 3]
		- matrix[ROWS_NUM * (N - 1)] * beta[N - 2])
		/ (matrix[ROWS_NUM * (N - 1)] * alpha[N - 2]
			+ matrix[ROWS_NUM * (N - 1) + 1]);  // x_N

	int i = N - 2;
	while (i >= 0) {
		x[i] = (alpha[i] * x[i + 1] + beta[i]);  // x_i
		i--;
	}
	return x;
}