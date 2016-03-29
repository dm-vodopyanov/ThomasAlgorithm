#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>

#define ROWS_NUM    4

using namespace std;

void help() {
    printf("This is a Thomas algorithm demo application [SEQUINTAL].\n\n");
    printf("You should provide arguments in the following format:\n\n");
    printf("  <inputfile> <outputfile> <timefile>\n\n");
    printf("NOTE: the input matrix should be the following:\n");
    printf("N 4\n");
    printf("0  c1 b1 f1\n");
    printf("a2 c2 b2 f2\n");
    printf("a3 c3 b3 f3\n");
    printf("a4 c4 b4 f4\n");
    printf(".. .. .. ..\n");
    printf("aN cN 0  fN\n");
}

vector<double> parseInputFile(FILE* inputFile, int& N) {
    int rows = 0;
    double number = 0;
    vector<double> matrix;
    fscanf(inputFile, "%d", &N);
    fscanf(inputFile, "%d", &rows);

    for (int i = 0; i < N * rows; i++) {
        fscanf(inputFile, "%lf ", &number);
        matrix.push_back(number);
    }

    fclose(inputFile);

    return matrix;
}

void writeToFile(char* fileName, vector<double> x, int N) {
    FILE* outputFile;
    outputFile = fopen("outputfile", "wt");
    fprintf(outputFile, "%d\n", N);
    for (int i = 0; i < N; i++) {
        fprintf(outputFile, "%lf\n", x[i]);
    }
    fclose(outputFile);
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

    FILE* inputFile;
    inputFile = fopen("inputfile", "rt");
    if (inputFile == NULL) {
        printf("ERROR: File %s not found\n", argv[1]);
        return -1;
    }

    int N = 0;

    vector<double> matrix;
    matrix = parseInputFile(inputFile, N);

    vector<double> alpha;
    vector<double> beta;
    vector<double> x;

    //alpha[0] = -b[0] / c[0];
    //beta[0]  = -f[0] / c[0];
    alpha.push_back(-matrix[2] / matrix[1]);
    beta.push_back(-matrix[3] / matrix[1]);

    for (int i = 1; i < N; i++) {
        //alpha[i] = -b[i - 1] / (a[i - 1] * alpha[0] + c[i - 1]);
        //beta[i] = (f[i - 1] - a[i - 1] * beta[i - 1]) / (a[i - 1] * alpha[i - 1] + c[i - 1]);
        alpha.push_back(-matrix[ROWS_NUM * (i - 1) + 2] / (matrix[ROWS_NUM * (i - 1)] + alpha[i - 1] + matrix[ROWS_NUM * (i - 1) + 1]));
        beta.push_back((matrix[ROWS_NUM * (i - 1) + 3] - matrix[ROWS_NUM * (i - 1)] * beta[i - 1]) 
                                                   / (matrix[ROWS_NUM * (i - 1)] * alpha[i - 1] + matrix[ROWS_NUM * (i - 1) + 1]));
    }

    //x[N - 1] = (f[N - 1] - a[N - 1] * beta[N - 1]) / (a[N - 1] * alpha[N - 1] + c[N - 1]);
    x.push_back((matrix[ROWS_NUM * (N - 1) + 3] - matrix[ROWS_NUM * (N - 1)] * beta[N - 1]) / (matrix[ROWS_NUM * (N - 1)] * alpha[N - 1] + matrix[ROWS_NUM * (N - 1) + 1]));

    int i = N - 2;
    int j = 0;
    while (i >= 0) {
        x.push_back(alpha[i + 1] * x[j] + beta[i + 1]);
        i--;
        j++;
    }

    reverse(x.begin(), x.end());

    writeToFile(argv[2], x, N);

    return 0;
}