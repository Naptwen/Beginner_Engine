#include "EMATRIX.h"
#include <device_launch_parameters.h>

#include <cuda_runtime.h>
#include <cuda.h>

void ADDMATRIX(EMATRIX* A, EMATRIX* B, EMATRIX* C)
{
	int Am = A->m;
	int An = A->n;
	int Bm = B->m;
	int Bn = B->n;
	int Cm = A->m;
	int Cn = A->n;
	int size_A = Am * An;
	int size_B = Bm * Bn;
	int size_C = Cm * Cn;
	int* A_gpu;
	int* B_gpu;
	int* C_gpu;

	cudaMalloc(&A_gpu, sizeof(int) * size_A);
	cudaMalloc(&B_gpu, sizeof(int) * size_B);
	cudaMalloc(&C_gpu, sizeof(int) * size_C);

	cudaMemcpy(A_gpu, A->vectors, sizeof(int) * size_A, cudaMemcpyHostToDevice);
	cudaMemcpy(B_gpu, B->vectors, sizeof(int) * size_B, cudaMemcpyHostToDevice);
	cudaMemcpy(C->vectors, C_gpu, sizeof(int) * size_C, cudaMemcpyDeviceToHost);

	cudaFree(A_gpu);
	cudaFree(B_gpu);
	cudaFree(C_gpu);
}

void CUDAMULTI(EMATRIX* A, EMATRIX* B)
{
	if (A->n == B->m)
	{
		//A = m x n and B = n x p So C=AB is  m x p
		int Am = A->m;
		int An = A->n;
		int Bm = B->m;
		int Bn = B->n;
		int Cm = A->m;
		int Cn = B->n;
		int size_A = Am * An;
		int size_B = Bm * Bn;
		int size_C = Cm * Cn;
		int* A_gpu;
		int* B_gpu;
		int* C_gpu;

		cudaMalloc(&A_gpu, sizeof(int) * size_A);
		cudaMalloc(&B_gpu, sizeof(int) * size_B);
		cudaMalloc(&C_gpu, sizeof(int) * size_C);
		cudaMemcpy(A_gpu, A->vectors, sizeof(int) * size_A, cudaMemcpyHostToDevice);
		cudaMemcpy(B_gpu, B->vectors, sizeof(int) * size_B, cudaMemcpyHostToDevice);

		int x = 0, y = 0, k = 0;
		for (int i = 0; i < size_C; i++)
		{
			x = i / Cm; //Why it is different with the coordinate position of vecotr2D on screen?
			y = i % Cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
			int sum = 0;
			for (int k = 0; k < A->n; k++) // from 0 to n
			{
				int index_A = k + x * An; //Axk
				int index_B = y + k * Bn;     //Bky	
				int iA, iB;
				cudaMemcpy(&iA, &A_gpu[index_A], sizeof(int), cudaMemcpyDeviceToHost);
				cudaMemcpy(&iB, &B_gpu[index_B], sizeof(int), cudaMemcpyDeviceToHost);
				sum += iA * iB;
			} //Cxy = Sigma (Axk * Bky)
			cudaMemcpy(&C_gpu[i], &sum, sizeof(int), cudaMemcpyHostToDevice);
		}
		if (A->vectors != NULL)
			free((void*)A->vectors);
		A->vectors = (float*)calloc(size_C, sizeof(float));
		cudaMemcpy(A->vectors, C_gpu, sizeof(int) * size_C, cudaMemcpyDeviceToHost);
		cudaFree(A_gpu);
		cudaFree(B_gpu);
		cudaFree(C_gpu);
		A->m = Cm;
		A->n = Cn;
	}
	else
		cout << "ERROR : MATRIX SIZE IS NOT SAME" << endl;
}
void CUDASUM(EMATRIX* A, EMATRIX* B)
{
	int Am = A->m;
	int An = A->n;
	int Bm = B->m;
	int Bn = B->n;
	int size_A = Am * An;
	int size_B = Bm * Bn;
	int* A_gpu;
	int* B_gpu;
	cudaMalloc(&A_gpu, sizeof(int) * size_A);
	cudaMalloc(&B_gpu, sizeof(int) * size_B);
	cudaMemcpy(A_gpu, A->vectors, sizeof(int) * size_A, cudaMemcpyHostToDevice);
	cudaMemcpy(B_gpu, B->vectors, sizeof(int) * size_B, cudaMemcpyHostToDevice);
	//kerneralAdd <<< k, A->n >> > (A_gpu, B_gpu, C_gpu);
}
void CUDATRANS(EMATRIX* A)
{
	int size_A = A->m * A->n;
	int x = 0, y = 0, temp_m = 0, j = 0, k = 0;
	if (A->m < A->n)
		temp_m = A->n;
	else
		temp_m = A->m;
	int length_temp = temp_m * temp_m; //TO make a n x n matrix for calculate
	int* AT;
	int* B;
	cudaMalloc(&AT, sizeof(int) * size_A);
	cudaMalloc(&B, sizeof(int) * size_A);
	cudaMemcpy(B, A->vectors, sizeof(int) * size_A, cudaMemcpyHostToDevice);
	//Copy from cpu to gpu
	for (int i = 0; i < length_temp; i++)
	{
		x = i / A->n;
		y = i % A->n;
		j = x + y * A->m; //reverse
		if (k < size_A) // if it filled all the capacity we don't need to do more.
		{
			cudaMemcpy(&AT[j], &B[y + x * A->n], sizeof(int), cudaMemcpyDeviceToDevice);
			k++;
		}
	}
	int temp = A->m;
	A->m = A->n;
	A->n = temp;
	cudaMemcpy(A->vectors, AT, sizeof(int) * size_A, cudaMemcpyDeviceToHost);
	cudaFree(AT);
	cudaFree(B);
}
