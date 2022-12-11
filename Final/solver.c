#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


int min(int num1, int num2){
    return (num1 > num2 ) ? num2 : num1;
}


int max(int num1, int num2){
    return (num1 > num2 ) ? num1 : num2;
}


void generate_x_column(int **X, int **M, int slot, int cClass ,int nOptions){
	for (int i = 0; i < nOptions; i++){
		X[i][slot] = M[i][cClass];
	}
}


int violations_per_option(int **X, int *P, int* Q, int depth, int i){
	int nViolations = 0;
	int summ, subs;
	for (int j = 0; j < depth - Q[i] + 2; j++){
		summ = 0, subs = 0;
		for (int k = 0; k < Q[i]; k++){
			summ = summ + X[i][k + j];
		}
		subs = summ - P[i];
		nViolations = nViolations + (X[i][j] * max(subs, 0));
	}
	return nViolations;
}


int total_violations(int **X, int *P, int *Q, int depth, int nOptions){
	int totalViolations = 0;
	for (int i = 0; i < nOptions; i++){
		totalViolations = totalViolations + violations_per_option(X, P, Q, depth, i);
	}
	return totalViolations;
}

void print_solution(int *S, int nCars){
	for (int i = 0; i < nCars; i++){
		printf("%d ", S[i]);
	}
	printf("\n");
}


// CS solver using recursive BT
void bt_solver(int **X, int **M, int *S, int *P, int *Q, int *D, int depth, int nCars, int nClasses, int nOptions){
	if (depth == nCars){
		print_solution(S, nCars);
		exit(0);
	}else{
		for (int i = 0; i < nClasses; i++){
			if(D[i] != 0){
				D[i]--;
				S[depth] = i;
				generate_x_column(X, M, depth, S[depth], nOptions);
				int violations = total_violations(X, P, Q, depth, nOptions);
				if(violations == 0){
					bt_solver(X, M, S, P, Q, D, depth+1, nCars, nClasses, nOptions);
				}
				D[i]++;
			}
		}
	}
}

int main(int argc, char** argv){
	FILE *fp;
	if(argc != 2){
		printf("Argumentos usados de forma incorrecta\n");
		return 1;
	}
	char filename[100];
	strcpy(filename, argv[1]);
	fp = fopen(filename ,"r");
	if (fp == NULL){
		printf("No se pudo leer el archivo\n");
		return 1;
	}

	int nCars, nOptions, nClasses; 
	fscanf(fp, "%d %d %d", &nCars, &nOptions, &nClasses);

	int P[nOptions], Q[nOptions];	
	int D[nClasses];			
	int S[nCars];			
	int **M, **X; 
	
	M = (int**)malloc(nOptions * sizeof(*M));
	X = (int**)malloc(nOptions * sizeof(*X));
	for (int i = 0; i < nOptions; i++){
		M[i] = (int*)malloc(nClasses * sizeof(*M[i]));
		X[i] = (int*)malloc(nCars * sizeof(*X[i]));
	}
	
	int num;
	for(int i = 0; i < nOptions; i++){
		fscanf(fp, "%d ", &num);
		P[i] = num;
	}
	for(int i = 0; i < nOptions; i++){
		fscanf(fp, "%d ", &num);
		Q[i] = num;
	}
	for(int i = 0; i < nClasses; i++){
		fscanf(fp, "%d ", &num);
		fscanf(fp, "%d ", &num);
		D[i] = num;
		for(int j = 0; j < nOptions; j++){
			fscanf(fp, "%d ", &num);
			M[j][i] =  num;	
		}
	}
	fclose(fp);
	// file read


	bt_solver(X, M, S, P, Q, D, 0, nCars, nClasses, nOptions);
	printf("No solution\n");

	return 0;
}
