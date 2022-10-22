#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int Try = 0;
int nViC = (int)INFINITY;

// retorna el minino entre el numero num1 y num2
int min(int num1, int num2){
    return (num1 > num2 ) ? num2 : num1;
}

// retorna el maximo entre el numero num1 y num2
int max(int num1, int num2){
    return (num1 > num2 ) ? num1 : num2;
}

// libera los dos arrays creados al inicio de la ejecucion del programa
void liberate_mem(int **M, int **X, int nOpciones){
	for (int i = 0; i < nOpciones; i++){
		free(X[i]);
		free(M[i]);
	}
	free(X);
	free(M);
}

// funcion que solo altera una columna de X en vez de alterar todo X como lo hace X_matrix_generator
// creada para optimizar un poco la ejecucion del programa.
void alter_column(int **X, int **M, int slot, int clase ,int nOpciones){
	for (int i = 0; i < nOpciones; i++){
		X[i][slot] = M[i][clase];
	}
}

// genera la matriz X usando S como base
void X_matrix_generator(int **X, int **M, int *S, int nAutos, int nOpciones){
	for (int i = 0; i < nOpciones; i++){
		for (int j = 0; j < nAutos; j++){
			X[i][j] = M[i][S[j]];
		}
	}
	return;
}

// retorna la cantidad de violaciones de capacidad para la estacion de la opcion i
int violations_per_option(int **X, int *P, int* Q, int nAutos, int i){
	// El parametro i representa la opcion.
	int nViolations = 0;
	int suma, resta, uLimit;
	for (int j = 0; j < nAutos; j++){
		uLimit = min(j+Q[i], nAutos);
		suma = 0, resta = 0;
		for (int k = j; k < uLimit; k++){
			suma = suma + X[i][k];
		}
		resta = suma - P[i];
		nViolations = nViolations + (X[i][j] * max(resta, 0));
	}
	return nViolations;
}


// retorna la suma de las violaciones de todas las opciones
int total_violations(int **X, int *P, int *Q, int nAutos, int nOpciones){
	int nViolations = 0;
	for (int i = 0; i < nOpciones; i++){
		nViolations = nViolations + violations_per_option(X, P, Q, nAutos, i);
	}
	return nViolations;
}


// funcion que copia una no solucion con la menor cantidad de violaciones a el array C.
void new_best_list(int *S, int *C, int nAutos){
	for (int i = 0; i < nAutos; i++){
		C[i] = S[i];
	}
	return;
}

// funcion que escribe el .out
void write_solution(int **X, int **M, int *S, int *P, int *Q, int nAutos, int nOpciones, clock_t initTime, clock_t endTime, char *oFilename){
	X_matrix_generator(X, M , S, nAutos, nOpciones);
	FILE *fp;
	fp = fopen(oFilename, "w");
	for (int i = 0; i < nAutos; i++){
		fprintf(fp,"%d ", S[i]);
	}
	fprintf(fp, "\n");
	int total_vi = total_violations(X, P, Q, nAutos, nOpciones);
	fprintf(fp,"%d\n", total_vi);
	int vi_per_o, n;
	for (int i = 0; i < nOpciones; i++){
		vi_per_o = violations_per_option(X, P, Q, nAutos, i);
		n = i + 1;
		fprintf(fp, "%d %d\n", n, vi_per_o);
	}
	clock_t totalTime = endTime - initTime;
	float finalTime = (float)((double) totalTime)/CLOCKS_PER_SEC;
	fprintf(fp, "%f[s]\n", finalTime);
	fclose(fp);
}

// solver del CS usando backtracking recursivo.
void bt_solver(int **X, int **M, int *S, int *C,int *P, int *Q, int *N, int *D, int profundidad, int nAutos, int nClases, int nOpciones, clock_t initTime, char *oFilename){
	if (profundidad == nAutos){
		//generate x
		//X_matrix_generator(X, M, S, nAutos, nOpciones);
		//check x
		int nVi = total_violations(X, P, Q, nAutos, nOpciones);
		Try++;
		if(nVi < nViC){
			nViC = nVi;
			new_best_list(S, C, nAutos);
		}
		if((Try%1000000) == 0 ){
			clock_t t2 = clock(); 
			write_solution(X, M, C, P, Q, nAutos, nOpciones, initTime, t2, oFilename);
		}
		if (nVi == 0){
			clock_t t2 = clock();
			write_solution(X, M, S, P, Q, nAutos, nOpciones, initTime, t2, oFilename);
			liberate_mem(M, X, nOpciones);
			exit(0);
		}
	}else{
		for (int i = 0; i < nClases; i++){
			if(D[i] != 0){
				D[i]--;
				S[profundidad] = N[i];
				alter_column(X, M, profundidad, S[profundidad], nOpciones);
				bt_solver(X, M, S, C, P, Q, N, D, profundidad+1, nAutos, nClases, nOpciones, initTime, oFilename);
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
	
	char txtFilename[100], oFilename[100];
	strcpy(txtFilename, argv[1]);
	strcat(txtFilename, ".txt");
	fp = fopen(txtFilename ,"r");
	if (fp == NULL){
		printf("No se pudo leer el archivo\n");
		return 1;
	}


	int nAutos, nOpciones, nClases;  //nAutos: N, nOpciones: O, nClases: C
	fscanf(fp, "%d %d %d", &nAutos, &nOpciones, &nClases);

	int P[nOpciones], Q[nOpciones];			// p/q
	int D[nClases], N[nClases];				// demanda cada clase - numero de la clase
	int S[nAutos], C[nAutos];				// slots, solucion candidata en el caso de que no existan soluciones
	int **M, **X;   // Matriz de opciones y final
	
	M = (int**)malloc(nOpciones * sizeof(*M));
	X = (int**)malloc(nOpciones * sizeof(*X));
	for (int i = 0; i < nOpciones; i++){
		M[i] = (int*)malloc(nClases * sizeof(*M[i]));
		X[i] = (int*)malloc(nAutos * sizeof(*X[i]));
	}
	
	int num;
	for(int i = 0; i < nOpciones; i++){
		fscanf(fp, "%d ", &num);
		P[i] = num;
	}
	for(int i = 0; i < nOpciones; i++){
		fscanf(fp, "%d ", &num);
		Q[i] = num;
	}
	for(int i = 0; i < nClases; i++){
		fscanf(fp, "%d ", &num);
		N[i] = num;
		fscanf(fp, "%d ", &num);
		D[i] = num;
		for(int j = 0; j < nOpciones; j++){
			fscanf(fp, "%d ", &num);
			M[j][i] =  num;	
		}
	}
	fclose(fp);
	// file read
	
	strcpy(oFilename, argv[1]);
	strcat(oFilename, ".out");

	time_t t1 = clock();
	bt_solver(X, M, S, C, P, Q, N, D, 0, nAutos, nClases, nOpciones, t1, oFilename);
	time_t t2 = clock();

	write_solution(X, M, C, P, Q, nAutos, nOpciones, t1, t2, oFilename);

	liberate_mem(M, X, nOpciones);
	
	// TODO: guardar solucion candidata: LISTO
	// TODO: printear solucion candidata cada 10000000 de intentos: LISTO
	// TODO: medir tiempo de ejecucion: LISTO
	// TODO: escribir los resultados en un .out: LISTO
	// TODO: liberar memoria: LISTO
	// ---- min ---
	// TODO: si el programa se termina con ctrl+c no se libera mem creo
	// TODO: optimizar x_matrix_generator ??

	//debug
	/*
	for (int i = 0; i < nAutos; i++){
		S[i] = 5;
	}
	X_matrix_generator(X, M, S, nAutos, nOpciones);
	int nvi = violations(X, P, Q, nAutos, nOpciones);
	printf("%d\n", nvi);
	for (int i = 0; i < nOpciones; i++){
		for (int j = 0; j < nAutos; j++){
			printf("%d ", X[i][j]);
		}
		printf("\n");	
	}

	for (int i = 0; i < nClases; i++){
		printf("%d", N[i]);
	}
	*/

	return 0;
}
