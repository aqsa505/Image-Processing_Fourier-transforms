#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define PI 3.1415927
#define MAG(i) (sqrt(Fr[i]*Fr[i] + Fi[i]*Fi[i]))
#define PHASE(i) (atan2(Fi[i],Fr[i]))

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

	FILE *input;
	input = fopen(inputFileName, "r");

	int inputWidth;
	int N;

	if(input == NULL){
		printf("Error reading file\n");
		exit(0);
	}

	// Retrieve first two number, which is width and N
	fscanf(input, "%d", &inputWidth);
	fscanf(input, "%d", &N);

	// Declare variables needed for DFT
	int u, x;
	double c, s, real, imag, cos(), sin();
	double Fr[N], Fi[N], fr[N], fi[N];

	// Forward DFT array set up
	if(mode == 0){
		// Store single width (real only) input onto array
		if(inputWidth == 1){
			for(int i = 0; i < N; i++){
				fscanf(input, "%lf", &fr[i]);
			}

			for(int i = 0; i < N; i++){
				fi[i] = 0;
			}
		}

		// Store two width (real and imaginary) input onto array
		if(inputWidth == 2){
			for(int i = 0; i < N; i++){
				fscanf(input, "%lf", &fr[i]);
				fscanf(input, "%lf", &fi[i]);
			}
		}
	} else if(mode == 1){
		// Inverse DFT array set up
		if(inputWidth == 2){
			for(int i = 0; i < N; i++){
				fscanf(input, "%lf", &Fr[i]);
				fscanf(input, "%lf", &Fi[i]);
			}
		} else {
			printf("Something wrong with your input file\n");
			exit(0);
		}
	} else {
		printf("Wrong mode\n");
		exit(0);
	}

	// Close input file
	fclose(input);

	// Perform forward DFT
	if(mode == 0){
		// Perform Fourier Analysis (DFT)
		for(u = 0; u < N; u++){
			real = imag = 0;

			for(x = 0; x < N; x++){
				c = cos(2. * PI * u * x / N);
				s = -sin(2. * PI * u * x / N);
				real += (fr[x] * c - fi[x] * s);
				imag += (fr[x] * s + fi[x] * c);
			}

			Fr[u] = real / N;
			Fi[u] = imag / N;
		}
	} else {
		// Perform Fourier Synthesis (DFT)
		for(x = 0; x < N; x++){
			real = imag = 0;
			for(u = 0; u < N; u++){
				c = cos(2. * PI * u * x / N);
				s = sin(2. * PI * u * x / N);
				real += (Fr[u] * c - Fi[u] * s);
				imag += (Fr[u] * s - Fi[u] * c);
			}

			fr[x] = real;
			fi[x] = imag;
		}
	}

	// Open output file for writing
	FILE *output;
	output = fopen(outputFileName, "w");

	if(output == NULL){
		printf("Error writing file\n");
		exit(0);
	}

	// Write first two column (width, N) of the file
	int outputWidth = 2;
	fprintf(output, "%d %d\n", outputWidth, N);

	// Write fourier coefficients to output file
	if(mode == 0){
		for(int i = 0; i < N; i++){
			fprintf(output, "%lf %lf\n", Fr[i], Fi[i]);
		}
	} else {
		for(int i = 0; i < N; i++){
			fprintf(output, "%lf %lf\n", fr[i], fi[i]);
		}
	}

	// Close output file
	fclose(output);

	return 0;
}