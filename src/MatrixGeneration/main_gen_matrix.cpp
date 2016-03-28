#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <vector>

#define ROWS_NUM    4
#define EPS         1

using namespace std;

void help() {
    printf("This is a tridiagonal matrix generation application.\n\n");
    printf("You should provide arguments in the following format:\n\n");
    printf("  <dimension> <minimum> <maximum>\n\n");
    printf("where  <dimension>  is dimension of the matrix,\n");
    printf("       <minimum>    is the smallest number in the matrix,\n");
    printf("       <maximum>    is the largest number in the matrix.\n\n");
    printf("Example: The output matrix is the following:\n");
    printf("N 4\n");
    printf("0  c1 b1 f1\n");
    printf("a2 c2 b2 f2\n");
    printf("a3 c3 b3 f3\n");
    printf("a4 c4 b4 f4\n");
    printf(".. .. .. ..\n");
    printf("aN cN 0  fN\n");

}

double random(int a, int b) {
    return (double)rand() / (double)RAND_MAX * (b - a) + a;
}

vector<double> generateCoefficients(int currentN, int N, int min, int max) {
    double a = 0, b = 0, c = 0, f = 0;
    double aPlusb = 0;
    vector<double> coeffs;
    
    // a
    if (currentN == 0)  // |c1|>=|b1|
        a = 0;  // a1
    else
        a = random(min, max);

    // b
    if (currentN == N - 1)  // |cN|>=|aN|
        b = 0;  // b
    else
        b = random(min, max);

    aPlusb = fabs(a) + fabs(b);

    // c
    if ((aPlusb > max) || ((max - aPlusb) < EPS))
        c = max + fabs(random(min, max));
    else {
        c = random(min, max);
        while (fabs(c) < aPlusb) {
            c = random(min, max);
        }
    }

    // f
    f = random(min, max);

    coeffs.push_back(a);
    coeffs.push_back(c);
    coeffs.push_back(b);
    coeffs.push_back(f);

    return coeffs;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        help();
        return -1;
    }
    else if (argc != 4) {
        printf("ERROR: Should be 3 arguments.\n\n");
        help();
        return -1;
    }

    int N = atoi(argv[1]);
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);

    printf("Dimension is %d\n", N);

    FILE* f;
    f = fopen("inputfile", "wt");

    srand(time(NULL));

    fprintf(f, "%d %d\n", N, ROWS_NUM);
    for (int i = 0; i < N; i++) {
        vector<double> coeffs = generateCoefficients(i, N, min, max);
        for (int j = 0; j < ROWS_NUM; j++) {
            fprintf(f, "%lf ", coeffs[j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Matrix generated successfully\n");

    return 0;
}