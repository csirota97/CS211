#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** multiply_matrices(int**, int**, int, int, int, int);
void freeAll(int**, int, int);
void writeMatrix(FILE *fout, int **matrix, int m, int n);

double startTime, stopTime;

int main(int argc, char** argv) {

	FILE* fin;
	FILE* fout;

	/* open the file */
	fin = fopen(argv[1], "r");
	
	/* read the file */
	int m1 = 0;
	int n1 = 0;
	int m2 = 0;
	int n2 = 0;
	int i = 0;
	int j = 0;
	
	if(fscanf(fin, "%d %d", &m1, &n1) != 2) {
		printf("error\n");
		return 0;
	}
	
	int** array1 = (int**) malloc(m1 * sizeof(int*));
	/*
	for(i = 0; i < m1; i ++) {
		array1[i] = (int*) malloc(n1 * sizeof(int));
	}
	
	for(i = 0; i < m1; i ++) {
		for(j = 0; j < n1; j ++) {
			if(fscanf(fin, "%d", &array1[i][j]) != 1) {
				printf("error\n");
				return 0;
			}
			if(j == n1 - 1) {
				fscanf(fin, "\n");
			} else {
				fscanf(fin, " ");
			}
		}
	}
    */
	for(i = 0; i < m1; i ++) {
        array1[i] = (int*) malloc(n1 * sizeof(int));

		for(j = 0; j < n1; j ++) {
			if(fscanf(fin, "%d", &array1[i][j]) != 1) {
				printf("error\n");
				return 0;
			}
			if(j == n1 - 1) {
				fscanf(fin, "\n");
			} else {
				fscanf(fin, " ");
			}
		}
	}
	
	fscanf(fin, "\n");
	
	if(fscanf(fin, "%d %d", &m2, &n2) != 2) {
		printf("error\n");
		return 0;
	}
	
	int** array2 = (int**) malloc(m2 * sizeof(int*));
	/*
	for(i = 0; i < m2; i ++) {
		array2[i] = (int*) malloc(n2 * sizeof(int));
	}
	
	for(i = 0; i < m2; i ++) {
		
        for(j = 0; j < n2; j ++) {
			if(fscanf(fin, "%d", &array2[i][j]) != 1) {
				printf("error\n");
				return 0;
			}
			if(j == n2 - 1) {
				fscanf(fin, "\n");
			} else {
				fscanf(fin, " ");
			}
		}
	}
    */
	
	for(i = 0; i < m2; i ++) {
		array2[i] = (int*) malloc(n2 * sizeof(int));
		
        for(j = 0; j < n2; j ++) {
			if(fscanf(fin, "%d", &array2[i][j]) != 1) {
				printf("error\n");
				return 0;
			}
			if(j == n2 - 1) {
				fscanf(fin, "\n");
			} else {
				fscanf(fin, " ");
			}
		}
	}
	fclose(fin);
	
	/* add the matrices and print the result */
	if(m2 != n1) {
		printf("error\n");
		return 0;
	}
	
	startTime = clock();
	int** product = multiply_matrices(array1, array2, m1, n1, m2, n2);
	stopTime = clock();
	
	printf("%d * %d; SEQUENTIAL; %f secs\n", m1, n2, (stopTime-startTime)/CLOCKS_PER_SEC);
	
	fout = fopen("matrix_result.txt", "w");
	writeMatrix(fout, product, m1, n2);
	fclose(fout);
	
	freeAll(array1, m1, n1);
	freeAll(array2, m2, n2);
	freeAll(product, m1, n2);
	
	//print_matrix(product, m1, n2);
	return 0;
}

int** multiply_matrices(int** matrix1, int** matrix2, int m1, int n1, int m2, int n2) {
	if(n1 != m2) {
		printf("error\n");
		return NULL;
	}
	
	int i = 0;
	int j = 0;
	int k = 0;
	int** product = (int**) malloc(m1 * sizeof(int*));
	/*
	for(i = 0; i < m1; i ++) {
		product[i] = (int*) malloc(n2 * sizeof(int));
	}
	
	for(i = 0; i < m1; i ++) {
		for(j = 0; j < n2; j ++) {
			product[i][j] = 0;
			for(k = 0; k < n1; k ++) {
				product[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
    */
	for(i = 0; i < m1; i ++) {
		product[i] = (int*) malloc(n2 * sizeof(int));
	}
	int /*ii,*/jj,kk;
//	int isize = 500;
	int jsize = 64;
	int ksize = 64;
	
//    for (i = 0; i < m1; i += isize) {
for (i = 0; i < m1; i ++) {
		for (k = 0; k < n1; k += ksize) {
			for(j = 0; j < n2; j += jsize) {
				if (k == 0)product[i][j] = 0;
				printf("%d aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",j);
//				for (ii = i; ii < i+isize; ii++) {
//printf("%d\n",ii);
					for (kk = k; kk < k+ksize; kk++) {
						for (jj = j; jj < j+jsize; jj++) {
	//						if (kk == k)product[ii][jj] = 0;
//							printf("i:%d\nj:%d\nk:%d\n---------------\n",ii,jj,kk);
							product[i][jj] += matrix1[i][kk] * matrix2[kk][jj];
//							product[ii][jj] += matrix1[ii][kk] * matrix2[kk][jj];
						}
					}
//				}
			}
		}
	}/*
	for(i = 0; i < m1; i+=2) {
        product[i] = (int*) malloc(n2 * sizeof(int));
		for(k = 0; k < n1; k+=2) {
            int acc00=0;
            int acc01=0;
            int acc10=0;
            int acc11=0;
printf("%d\n",i);
			for(j = 0; j < n2; j+=2) {
//                printf("2\n");
              if (k == 0)product[i][j] = 0;
 //             product[i][j] += matrix1[i][k] * matrix2[k][j];
                acc00 += matrix1[i + 0][k] * matrix2[k][j + 0];
                acc01 += matrix1[i + 0][k] * matrix2[k][j + 1];
                acc10 += matrix1[i + 1][k] * matrix2[k][j + 0];
                acc11 += matrix1[i + 1][k] * matrix2[k][j + 1];
//                printf("3\n");
			}
//            printf("4\n");
            product[i + 0][j + 0] = acc00;
//            printf("5\n");
            product[i + 0][j + 1] = acc01;
//            printf("6\n");
            if(i+1<m1 && j+1<n2){
//				printf("9\n");
                product[i + 1][j + 0] = acc10;
//                printf("7\n");
                product[i + 1][j + 1] = acc11;
//                printf("8\n");
            }
		}
	}*/
	return product;
}

/* Freeing fcts ------------------------------------------------------- */
void freeAll(int **matrix, int m, int n) {
	int i;
	for (i=0; i<m; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void writeMatrix(FILE *fout, int **matrix, int m, int n) {
	int i,j;

	for(i=0; i<m; i++) {     
		for(j=0; j<n; j++)  
			fprintf(fout,"%d ",matrix[i][j]); 
		putc('\n',fout);
   }
}


