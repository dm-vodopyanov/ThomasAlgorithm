#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <omp.h>

#define ROWS_NUM    4

using namespace std;

vector<double> seqVersion(vector<double> matrix, int N)
{
    vector<double> alpha;
    vector<double> beta;
    vector<double> x;

    // SEQUINTAL VERSION STARTS =========================================================

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

vector<double> func(int N, vector<double> matrix) {
    int grainsize = 0; // порция уравненений потоку, с округлением в меньшую сторону
    int P = 0; // макс количество потоков

    vector<double> miniMatrix;

    vector<double> X;
    vector<double> intermediateX;
    vector<double> g(N);
    vector<double> d(N);

    g.assign(N, 0);
    d.assign(N, 0);
    X.assign(N, 0);
    //miniMatrix.resize(10000);
    
    int j, n, y, i, m, t, r, x;
    double coef;

    //double t1 = omp_get_wtime();

    //P = omp_get_num_threads();
    P = 4;
	intermediateX.assign(P, 0);
    grainsize = N / P;

    //#pragma omp parallel shared(matrix,miniMatrix,intermediateX,X,g,d,grainsize,P,N,t,r) private(m,x,coef, i, j, n, y)
    //{
    //P = omp_get_num_threads();
    //grainsize = N / P;

	int allNumThrds = 0;

	 omp_set_num_threads(4); 
#pragma omp parallel for shared(d, matrix) private (j, coef)
    for (j = 0; j < N; j++)
    {
		if(omp_get_thread_num() == 0)
			allNumThrds = omp_get_num_threads();
		//int currNumThrd = omp_get_thread_num();
		if (j % (grainsize)  == 0)
			d[j] = matrix[4 * j];
		else 
		{
			// coef = a[i + 1 + grainsize * CurP] / a[i + grainsize * CurP] 
			coef = matrix[j * 4] / matrix[1 + (j - 1) * 4];
			// d[i + 1 + grainsize * CurP] = - b[i + grainsize * CurP] * coef
			d[j] = -d[j - 1] * coef;
			// a[i + 1 + grainsize * CurP] = a[i + 1 + grainsize * CurP] - c[i + grainsize * CurP] * coef
			matrix[j * 4 + 1] = matrix[j * 4 + 1] - coef * matrix[(j - 1) * 4 + 2];
			// f[i + 1 + grainsize * CurP] = f[i + 1 + grainsize * CurP] - f[i + grainsize * CurP] * coef
			matrix[j * 4 + 3] = matrix[j * 4 + 3] - coef * matrix[j * 4 - 1];
			//обнуляем поддиагональные элементы, необязательно вроде как
			//matrix[P + 2        [63]    1.1148922154958965    double + j*grainsize*4] = 0; // 4 - количество столбцов b[2 + grainsize * CurP] = 0
		}
    }


#pragma omp parallel for shared(matrix, d, g) private(n, coef)
        for (n = 0; n < N; n++)
        {
            if (n % (grainsize)  == 0)
				g [N - n - 2] = matrix[4 * (N - n - 1) - 2];
			else 
			{
				if(n != N-1) {

					coef = matrix[4 * (N - n - 2) + 2] / matrix[4 * (N - n - 1) + 1];

					g[N - n - 2] = - g[(N - n - 2) + 1] * coef;
					if(n % (grainsize) == grainsize - 1)
						matrix[(N - n - 2) * 4 + 1] = matrix[(N - n - 2) * 4 + 1] - d[N - n - 1] * coef;
					else 
						d[N - n - 2] = d[N - n - 2] - d[(N - n - 2) + 1] * coef;

					matrix[(N - n - 2) * 4 + 3] = matrix[(N - n - 2) * 4 + 3] - coef * matrix[(N - n - 1) * 4 + 3];
				}
			}
        }

    miniMatrix.push_back(0);
    miniMatrix.push_back(matrix[4 * grainsize - 3]);
    miniMatrix.push_back(g[grainsize - 1]);
    miniMatrix.push_back(matrix[4 * grainsize - 1]);
    for (t = 2; t < P; t++)
    {
        miniMatrix.push_back(d[t * grainsize - 1]);
        miniMatrix.push_back(matrix[4 * t * grainsize - 3]); // a[grainsize * t]
        miniMatrix.push_back(g[t * grainsize - 1]);
        miniMatrix.push_back(matrix[4 * t * grainsize - 1]); // f[grainsize * t]
    }
    miniMatrix.push_back(d[N - 1]);
    miniMatrix.push_back(matrix[4 * N - 3]);
    miniMatrix.push_back(0);
    miniMatrix.push_back(matrix[4 * N - 1]);
    intermediateX = seqVersion(miniMatrix, P);

    for (r = 0; r < P; r++)
    {
        X[grainsize * (r + 1) - 1] = intermediateX[r];
    }
    
#pragma omp parallel for shared(matrix, X, d, g) private(y)
    for (y = 0; y < N; y++)
    {
        if (y % grainsize != 0)
		{
			if (y > N - grainsize)
				X[N - y - 1] = (matrix[N * 4 - y * 4 - 1] - g[N - y - 1] * X[grainsize - 1]) / matrix[N * 4 - y * 4 - 3];
			else
				X[N - y - 1] = (matrix[N * 4 - y * 4 - 1] - g[N - y - 1] * X[grainsize * (N / grainsize - y / grainsize) - 1] - d[N - y - 1] * X[grainsize * (N / grainsize - y / grainsize) - 1 - grainsize]) / matrix[N * 4 - y * 4 - 3];
		}
	}
    return X;
}