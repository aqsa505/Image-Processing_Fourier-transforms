#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Include fft1d algorithm from Prof.Wolberg
#include "fft1d.h"

int main(int argc, char *argv[]){
    // Retrieve arguments to main function
    if(argc != 4){
        printf("Unexpected number of arguments.\n");
        exit(0);
    }

    // Set arguments to variables
    char inputFileName[50];
    char outputFileName[50];
    strcpy(inputFileName, argv[1]);
    int mode = atoi(argv[2]);
    strcpy(outputFileName, argv[3]);

    FILE *inputFile;
    inputFile = fopen(inputFileName, "r");

    int inputWidth;
    int N;

    if(inputFile == NULL){
        printf("Error reading file\n");
        exit(0);
    }

    // Retrieve first two number, which is width and N
    fscanf(inputFile, "%d", &inputWidth);
    fscanf(inputFile, "%d", &N);

    float Fr[N], Fi[N], fr[N], fi[N];

    // Forward DFT array set up
    if(mode == 0){
        // Store single width (real only) input onto array
        if(inputWidth == 1){
            for(int i = 0; i < N; i++){
                fscanf(inputFile, "%f", &fr[i]);
            }

            for(int i = 0; i < N; i++){
                fi[i] = 0.0;
            }
        }

        // Store two width (real and imaginary) input onto array
        if(inputWidth == 2){
            for(int i = 0; i < N; i++){
                fscanf(inputFile, "%f", &fr[i]);
                fscanf(inputFile, "%f", &fi[i]);
            }
        }
    } else if(mode == 1){
        // Inverse DFT array set up
        if(inputWidth == 2){
            for(int i = 0; i < N; i++){
                fscanf(inputFile, "%f", &Fr[i]);
                fscanf(inputFile, "%f", &Fi[i]);
            }
        } else {
            printf("Something wrong with your input file\n");
            exit(0);
        }
    } else {
        printf("Wrong mode\n");
        exit(0);
    }

    complexS inputStruct = {N, fr, fi};
    complexS outputStruct = {N, Fr, Fi};

    complexP input = &inputStruct;
    complexP output = &outputStruct;

    if(mode == 0){
        fft1D(input, 0, output);
    } else {
        fft1D(output, 1, input);
    }

    fclose(inputFile);

    // Open output file for writing
    FILE *outputFile;
    outputFile = fopen(outputFileName, "w");

    if(outputFile == NULL){
        printf("Error writing file\n");
        exit(0);
    }

    // Write first two column (width, N) of the file
    int outputWidth = 2;
    fprintf(outputFile, "%d %d\n", outputWidth, N);

    // Write fourier coefficients to output file
    if(mode == 0){
        for(int i = 0; i < N; i++){
            fprintf(outputFile, "%lf %lf\n", Fr[i], Fi[i]);
        }
    } else {
        for(int i = 0; i < N; i++){
            fprintf(outputFile, "%lf %lf\n", fr[i], fi[i]);
        }
    }

    // Close output file
    fclose(outputFile);

    return 0;
}
