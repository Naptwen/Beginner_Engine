#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef TCUDA_MATRIX_H__
#define TCUDA_MATRIX_H__

#include "TMATRIX.h"

#define MULTI 197
#define ADD_COL 201
#define ADD_ROW 202
#define TRANS 205
#define CUT_COL 211
#define CUT_ROW 218
#define COPY_COL 220

//A front matrix B is back matrix
//m -> C's m
//p -> A's n
//n -> C's n
template<typename vtype>
__global__
void TCUDA_MATRIX_MULTI(int m, int p, int n, int trash, vtype* A, vtype* B, vtype* C)
{
	int k = threadIdx.x + blockIdx.x * blockDim.x; //index of C

	if (k < m * n) {
		vtype temp = 0;
		for (int t = 0; t < p; t++) {
			temp += A[t + k / n * p] * B[k % n + t * n];
		}
		C[k] = temp;
	}
}
//A front matrix B is back matrix
//m -> A's m or B's m
//nC -> C's n or B's m
//nA -> A's n
template<typename vtype>
__global__
void TCUDA_MATRIX_ADD_COLUMN(int m, int nC, int nA, int trash, vtype* A, vtype* B, vtype* C)
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
template<typename vtype>
__global__
void TCUDA_MATRIX_ADD_ROW(int mC, int nC, int mA, int trash, vtype* A, vtype* B, vtype* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int mnC = mC * nC;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		if (x < mA)
			C[y + x * nC] = A[y + x * nC];
		else
			C[y + x * nC] = B[y + (x - mA) * nC];
	}
}
//MULTI : A*B
//ADD_ROW : A + B vertically add extra column
//ADD_COL : A + B horizontally add extra rows
//TRANS : A^T , B is NULL, 
template<typename vtype>
__global__
void TCUDA_MATRIX_TRANS(int mC, int nC, int  mnC, int trash, vtype* A, vtype* trash2, vtype* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < mnC)
	{
		int x = i / nC;
		int y = i % nC;
		C[y + x * nC] = A[x + y * mC];
	}
}
//nA is # column in A 
//start column number remember 0 is 1, 1 is 2
//END column number remember 0 is 1, 1 is 2
//B is NULL
template<typename vtype>
__global__
void TCUDA_MATRIX_CUT_COL(int mA, int nA, int start, int end, vtype* A, vtype* trahs, vtype* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int num = mA * nA;
	if (i < num)
	{
		int x = i / nA;
		int y = i % nA;
		if (y >= start && y <= end)
			C[y - start + x * (end - start + 1)] = A[i];
	}
}
//start row number remember 0 is 1, 1 is 2
//END row number remember 0 is 1, 1 is 2
//B is NULL
template<typename vtype>
__global__
void TCUDA_MATRIX_CUT_ROW(int mA, int nA, int start, int end, vtype* A, vtype* trash, vtype* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int num = mA * nA;
	if (i < num)
	{
		int x = i / nA;
		int y = i % nA;
		if (x >= start && x <= end)
			C[y + (x - start) * nA] = A[i];
	}
}
//end is the index of A ex)if you want to increase 1 more end must be 1
template<typename vtype>
__global__
void TCUDA_MATRIX_COPY_COL(int mA, int nA, int nC, int end, vtype* A, vtype* trash, vtype* C)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int num = mA * nA;
	if (i < num)
	{
		for (int k = 0; k <= end; k++)
		{
			C[k + i * nC] = A[i];
		}
	}
}
//start and end are only used for the MATRIX CUT 
//So in the other case let it as zero or null
//*[MULTI]   :need A, B	*[CUT_COL] : need start end  
//*[ADD_COL] :need A, B	*[CUT_ROW] : need start end
//*[ADD_ROW] :need A, B	*[COPY_COL]: need end(end is index)
//*[TRANS]   :need Only A	
template<typename vtype>
TMATRIX<vtype>* TCUDA_MATRIX_CONVERT(
	TMATRIX<vtype>* A, TMATRIX<vtype>* B, 
	int start, int end, const unsigned char FUN)
{
	vtype* CUDA_A = NULL;
	vtype* CUDA_B = NULL;
	vtype* CUDA_C = NULL;
	int size_A = 0, size_B = 0, size_C = 0;
	if (A != nullptr)
	{

		size_A = sizeof(vtype) * A->m * A->n;
		cudaMalloc(&CUDA_A, size_A);
		cudaMemcpy(CUDA_A, A->vectors, size_A, cudaMemcpyHostToDevice);
	}
	if (B != nullptr)
	{

		size_B = sizeof(vtype) * B->m * B->n;
		cudaMalloc(&CUDA_B, size_B);
		cudaMemcpy(CUDA_B, B->vectors, size_B, cudaMemcpyHostToDevice);

	}
	TMATRIX<vtype>* C = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
	void (*temp)(int, int, int, int, vtype*, vtype*, vtype*);
	long int bx = 0, by = 0, bz = 0, bi = 0;
	switch (FUN)
	{
	case MULTI:
		if (A->n != B->m)
		{
			//elog << "[ERROR] MULTI MATRIX:" + to_string(A->n) + to_string(B->m) + "is not equal\n";
			return nullptr;
		}
		temp = TCUDA_MATRIX_MULTI;
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
			//elog << "[ERROR] ADD COLUMN:" + to_string(A->m) + " : " + to_string(B->m) + "is not equal\n";
			return nullptr;
		}
		temp = TCUDA_MATRIX_ADD_COLUMN;
		size_C = A->n * A->m + B->n * A->m;//for the speed
		C->m = A->m;
		C->n = A->n + B->n;
		bx = C->m;
		by = C->n;
		bz = A->n;
		break;
	case ADD_ROW:
		if (A->n != B->n)
		{
			//elog << "[ERROR] ADD ROW:" + to_string(A->n) + " : " + to_string(B->n) + "is not equal\n";
			return nullptr;
		}
		temp = TCUDA_MATRIX_ADD_ROW;
		size_C = A->n * A->m + B->n * A->m;//for the speed
		C->m = A->m + B->m;
		C->n = A->n;
		bx = C->m;
		by = C->n;
		bz = A->m;
		break;
	case TRANS:
		temp = TCUDA_MATRIX_TRANS;
		size_C = A->n * A->m;//for the speed
		C->m = A->n;
		C->n = A->m;
		bx = C->m;
		by = C->n;
		bz = C->m * C->n;
		break;
	case CUT_COL:
		if (start <0 && end > A->n)
		{
			//elog << "[ERROR] CUT COL:" + to_string(start) + " : " + to_string(end) + "is out of range\n";
			return nullptr;
		}
		temp = TCUDA_MATRIX_CUT_COL;
		size_C = A->m * (end - start + 1);
		C->m = A->m;
		C->n = (end - start + 1);
		bx = A->m;
		by = A->n;
		bz = start;
		bi = end;
		break;
	case CUT_ROW:
		if (start <0 && end > A->m)
		{
			//elog << "[ERROR] CUT ROW:" + to_string(start) + " : " + to_string(end) + "is out of range\n";
			return nullptr;
		}
		temp = TCUDA_MATRIX_CUT_ROW;
		size_C = A->n * (end - start + 1);
		C->m = (end - start + 1);
		C->n = A->n;
		bx = A->m;
		by = A->n;
		bz = start;
		bi = end;
		break;
	case COPY_COL:
		temp = TCUDA_MATRIX_COPY_COL;
		size_C = (A->n + end) * A->m;
		C->m = A->m;
		C->n = (A->n + end);
		bx = A->m;
		by = A->n;
		bz = C->n;
		bi = end;
		break;
	}

	cudaMalloc(&CUDA_C, sizeof(vtype) * size_C);
	dim3 blockPerGrid(512, 1, 1);
	dim3 threadsPerBlock(512, 1, 1);

	temp <<<blockPerGrid, threadsPerBlock >>> (bx, by, bz, bi, CUDA_A, CUDA_B, CUDA_C);

	C->vectors = (vtype*)malloc(sizeof(vtype) * size_C);
	cudaMemcpy(C->vectors, CUDA_C, sizeof(vtype) * size_C, cudaMemcpyDeviceToHost);
	if (CUDA_A != NULL)
		cudaFree(CUDA_A);
	if (CUDA_B != NULL)
		cudaFree(CUDA_B);
	if (CUDA_C != NULL)
		cudaFree(CUDA_C);
	return C;
}

#endif