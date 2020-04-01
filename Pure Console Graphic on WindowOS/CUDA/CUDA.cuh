#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "EMATRIX.h"

#define MULTI 197
#define ADD_COL 201
#define ADD_ROW 202
#define TRANS 205
//A front matrix B is back matrix
//m -> C's m
//p -> A's n
//n -> C's n
__global__
void CUDA_MATRIX_MULTI(int m, int p, int n, float* A, float* B, float* C)
{
	int k = threadIdx.x + blockIdx.x * blockDim.x; //index of C

	if (k < m*n) {
		float temp = 0.0f;
		for (int t = 0; t < p; t++) {
			temp += A[t + k/n*p] * B[k % n + t * n]; 
		}
		C[k] = temp;
	}
}
//A front matrix B is back matrix
//m -> A's m or B's m
//nC -> C's n or B's m
//nA -> A's n
__global__ 
void CUDA_MATRIX_ADD_COLUMN(int m, int nC, int nA,  float* A, float* B, float* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int mnC = m * nC;
	int nB = nC - nA;
	if (i < mnC) 
	{
		int x = i / nC;
		int y = i % nC;
		if (y < nA)
			C[y + x * nC] = A[y + x * nA];
		else 
			C[y + x * nC] = B[y - nA + x * nB];
	}
}
//A front matrix B is back matrix
//mC -> C's m
//nC -> C'n or A'n or B'n
//mA -> A's m
__global__
void CUDA_MATRIX_ADD_ROW(int mC, int nC, int mA, float* A, float* B, float* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int mnC = mC * nC;
	int mB = mC - mA;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		if (x < mA)
			C[y + x * nC] = A[y + x * nC];
		else
			C[y + x * nC] = B[y + (x-mA) * nC];
	}
}
//MULTI : A*B
//ADD_ROW : A + B vertically add extra column
//ADD_COL : A + B horizontally add extra rows
//TRANS : A^T , B is NULL, 
__global__
void CUDA_MATRIX_TRANS(int mC, int nC, int  mnC, float* A, float* B, float* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		C[y + x * nC] = A[x + y * mC];
	}
}

EMATRIX* CUDA_MATRIX_CONVERT(EMATRIX* A, EMATRIX* B, const unsigned char FUN)
{
	float* CUDA_A = NULL;
	float* CUDA_B = NULL;
	float* CUDA_C = NULL;
	int size_A = 0, size_B = 0, size_C = 0;
	if (A != nullptr)
	{

		size_A = sizeof(float) * A->m * A->n;
		cudaMalloc(&CUDA_A, size_A);
		cudaMemcpy(CUDA_A, A->vectors, size_A, cudaMemcpyHostToDevice);
	}
	if (B != nullptr)
	{

		size_B = sizeof(float) * B->m * B->n;
		cudaMalloc(&CUDA_B, size_B);
		cudaMemcpy(CUDA_B, B->vectors, size_B, cudaMemcpyHostToDevice);

	}
	EMATRIX* C = (EMATRIX*)malloc(sizeof(EMATRIX));
	void (*temp)(int, int, int, float*, float*, float*);
	long int bx, by, bz;
	switch (FUN)
	{
	case MULTI:
		if (A->n != B->m)
		{
			elog << "[ERROR] MULTI MATRIX:" + to_string(A->n) + to_string(B->m) + "is not equal";
			return nullptr;
		}
		temp = CUDA_MATRIX_MULTI;
		size_C = A->m * B->n;
		C->m = A->m;
		C->n = B->n;
		bx = C->m;
		by = A->n;
		bz = C->n;
		break;
	case ADD_COL:
		if (A->m != B->m)
		{
				elog << "[ERROR] ADD COLUMN:" + to_string(A->m) + to_string(B->m) + "is not equal";
			return nullptr;
		}
		temp = CUDA_MATRIX_ADD_COLUMN;
		size_C = A->n * A->m + B->n * A->m;//for the speed
		C->m = A->m ;
		C->n = A->n + B->n;
		bx = C->m;
		by = C->n;
		bz = A->n;
		break;
	case ADD_ROW:
		if (A->n != B->n)
		{
			elog << "[ERROR] ADD ROW:" + to_string(A->n) + to_string(B->n) + "is not equal";
			return nullptr;
		}
		temp = CUDA_MATRIX_ADD_ROW;
		size_C = A->n * A->m + B->n * A->m;//for the speed
		C->m = A->m + B->m;
		C->n = A->n;
		bx = C->m;
		by = C->n;
		bz = A->m;
		break;
	case TRANS:
		temp = CUDA_MATRIX_TRANS;
		size_C = A->n * A->m;//for the speed
		C->m = A->n;
		C->n = A->m;
		bx = C->m;
		by = C->n;
		bz = C->m * C->n;
		break;
	}
	cudaMalloc(&CUDA_C, sizeof(float) * size_C);
	dim3 blockPerGrid(512, 1, 1);
	dim3 threadsPerBlock(512,1,1);
	temp << <blockPerGrid, threadsPerBlock >> > (bx,by,bz,CUDA_A,CUDA_B,CUDA_C);

	C->vectors = (float*)malloc(sizeof(float) * size_C);
	cudaMemcpy(C->vectors, CUDA_C, sizeof(float) * size_C, cudaMemcpyDeviceToHost);
	if(CUDA_A != NULL)
		cudaFree(CUDA_A);
	if (CUDA_B != NULL)
		cudaFree(CUDA_B);
	if (CUDA_C != NULL)
		cudaFree(CUDA_C);
	return C;
}
