#include "fifth.h"


int main(int argc, char **argv) {
	int col = 0;
	int row = 0;
	int ret = 0; //returns number of inputs after each scan

	if (argc != 2) { //Check if argv[1] is real
		printf("error\n");
		return 0;
	}
	
	FILE* fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL) { //ends program if file not found
		printf("error\n");
		return 0;
	}

	ret = fscanf(fp,"%d\t%d\n", &row, &col); //reads file and sets vars
	if (ret != 2) {
		printf("error\n");
		return 0;
	}

	int matA[row][col]; //create matrix A
	int i = 0;
	for (; i < row; i++) {
		int j = 0;
		for (; j < col; j++) {
			fscanf(fp, "%d",&(matA[i][j]));
		}
	}
	int matB[row][col]; //create matrix B
	i = 0;
	for (; i < row; i++) {
		int j = 0;
		for (; j < col; j++) {
			fscanf(fp, "%d",&(matB[i][j]));
		}
	}

	i = 0;
	for (; i < row; i++) { //print sum
		int j = 0;
		for (; j < col; j++) {
			printf("%d\t",matA[i][j] + matB[i][j]);
		}
		printf("\n");
	}
	return 0;
}
