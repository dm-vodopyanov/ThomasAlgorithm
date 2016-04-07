#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>

#define ROWS_NUM    4

using namespace std;

void help() {
    printf("This is a Thomas algorithm demo application [CILK].\n\n");
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

void writeResultToFile(char* fileName, vector<double> x, int N) {
    FILE* outputFile;
    outputFile = fopen("outputfile_cilk", "wt");
    fprintf(outputFile, "%d\n", N);
    for (int i = 0; i < N; i++) {
        fprintf(outputFile, "%lf\n", x[i]);
    }
    fclose(outputFile);
}

void writeTimeToFile(double resultTime) {
    FILE* timeFile;
    timeFile = fopen("timefile_cilk", "wt");
    fprintf(timeFile, "%lf\n", resultTime);
    fclose(timeFile);
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

    vector<double> x;

    LARGE_INTEGER frequency, start, end;
    double resultTime;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    // ADD YOUR CODE HERE

    QueryPerformanceCounter(&end);
    resultTime = (double)(end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;

    writeResultToFile(argv[2], x, N);
    writeTimeToFile(resultTime);

    return 0;
}