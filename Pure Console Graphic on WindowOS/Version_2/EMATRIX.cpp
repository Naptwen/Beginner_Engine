#pragma once
#include "EMATRIX.h"
#include <conio.h>
EMATRIX::EMATRIX()
{
	this->m = 0;
	this->n = 0;
	vectors = NULL;
}
EMATRIX::EMATRIX(int m, int n)
{
	this->m = m;
	this->n = n;
	vectors = (float*)calloc(m * n, sizeof(float));
}
EMATRIX::~EMATRIX()
{
	if (vectors != NULL)
		std::free(vectors);
	vectors = NULL;
}
void EMATRIX::operator = (const float* matrix)
{
	memmove(vectors, matrix, m * n * sizeof(float));
}
void EMATRIX::operator = (const EMATRIX &matrix)
{
	if (this->vectors != NULL)
		std::free((void*)vectors);
	this->vectors = (float*)calloc(matrix.m * matrix.n, sizeof(float));
	memmove((void*)(this->vectors), matrix.vectors, m * n * sizeof(float));
	this->m = matrix.m;
	this->n = matrix.n;
}
EMATRIX* EMATRIX::operator * (const  EMATRIX &B)
{
	EMATRIX* C = (EMATRIX*)malloc(sizeof(EMATRIX));
	if (this->n == B.m)
	{
		//A = m x n and B = n x p So C=AB is  m x p
		int Cm = this->m;
		int Cn = B.n;
		int size_C = Cm * Cn;
		float* matrix_C = (float*)calloc(size_C, sizeof(float));
		int x = 0, y = 0, k = 0;
		for (int i = 0; i < size_C; i++)
		{
			x = i / Cn; //Why it is different with the coordinate position of vecotr2D on screen?
			y = i % Cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
			float sum = 0;//														     (1,0)
			for (int k = 0; k < this->n; k++) // from 0 to n
			{
				int index_A = k + x * this->n; //Axk
				int index_B = y + k * B.n;     //Bky
				float temp = this->vectors[index_A] * B.vectors[index_B];

				sum += temp;
			}
			matrix_C[i] = sum; //Cxy = Sigma (Axk * Bky)
		}
		C->m = Cm;
		C->n = Cn;
		C->vectors = matrix_C;
		return C;
	}
	else
	{
		cout << "ERROR : MATRIX SIZE IS NOT SAME" << endl;
		return nullptr;
	}
}
EMATRIX* EMATRIX::operator * (const  float f)
{
	EMATRIX* C = (EMATRIX*)malloc(sizeof(EMATRIX));
	C->vectors = (float*)malloc(this->m * this->n * sizeof(float));
	memmove(C->vectors, this->vectors, this->m * this->n * sizeof(float));
	int length = this->m * this->n;
	for (int k = 0; k < length; k++)// from 0 to n
		C->vectors[k] = C->vectors[k] * f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
EMATRIX* EMATRIX::operator + (const  float f)
{
	EMATRIX* C = (EMATRIX*)malloc(sizeof(EMATRIX));
	C->vectors = (float*)malloc(this->m * this->n * sizeof(float));
	memmove(C->vectors, this->vectors, this->m * this->n * sizeof(float));
	int length = this->m * this->n;
	for (int k = 0; k < length; k++)
		C->vectors[k] = C->vectors[k] + f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
EMATRIX* EMATRIX::T()
{
	int size_A = m * n;
	int x = 0, y = 0, temp_m = 0,  j = 0, k = 0;
	//-----------------
	if (m < n)
		temp_m = n;
	else
		temp_m = m;
	int size_Square = temp_m * temp_m; //TO make a n x n matrix for calculate
	//-----------------
	float* AT = (float*)calloc(size_A, sizeof(float));
	for (int i = 0; i < size_Square; i++)
	{
		x = i / n;
		y = i % n;
		j = x + y * m; //reverse
		if (k < size_A) // if it filled all the capacity we don't need to do more.
		{
			AT[j] = vectors[y + x * n];
			k++;
		}
	}
	//-----------------
	EMATRIX* TRANSA = (EMATRIX*)malloc(sizeof(EMATRIX));
	TRANSA->m = this->n;
	TRANSA->n = this->m;
	TRANSA->vectors = AT;
	return TRANSA;
}
void EMATRIX::info()
{
	cout << "--------------------" << endl;
	int length = this->m * this->n;
	int y = 1;
	for (int i = 0; i < length; i++)
	{
		printf("%.4f,", this->vectors[i]);
		//cout << (float) this->vectors[i] << ", ";
		if (y == this->n)
		{
			cout << "\n";
			y = 1;
		}
		else
			y++;
	}
	cout << "--------------------" << endl;
}
void EMATRIX::m2txt(string name)
{
	int length = m * n;
	string out_line;
	ofstream out(name);
	int x = 1;
	for (int i = 0; i < length; i++)
	{
		out << this->vectors[i] << " , ";
		cout << this->vectors[i] << ", ";
		if (x == this->n)
		{
			out << "\n";
			cout << "\n";
			x = 1;
		}
		else
			x++;
	}
	out.close();
}
void EMATRIX::COLUMCUT(int st, int ed)
{
	if (ed - st > 0 && ed < this->n && st >= 0)
	{
		float* press = (float*)malloc(sizeof(float) * (ed - st + 1) * this->m);
		int k = 0;
		for (int j = 0; j < this->m; j++)
			for (int i = st; i <= ed; i++)
			{
				memmove(&press[k], &this->vectors[i + j * this->n], sizeof(float));
				k++;
			}
		free(this->vectors);
		this->vectors = press;
		this->n = ed - st + 1;
	}
	else
		cout << "ERROR:: size is not defined" << endl;
}
void EMATRIX::ROWCUT(int st, int ed)
{
	if (ed - st > 0 && ed < this->m && st >= 0)
	{
		float* press = (float*)malloc(sizeof(float) * (ed - st + 1) * this->n);
		int k = 0;
		for (int i = st; i <= ed; i++)
			for (int j = 0; j < this->n; j++)
			{
				memmove(&press[k], &this->vectors[j + i * this->n], sizeof(float));
				k++;
			}
		free(this->vectors);
		this->vectors = press;
		this->m = ed - st + 1;
	}
	else
		cout << "ERROR:: size is not defined" << endl;
}
void EMATRIX::ZEROS() 
{
	std::free(this->vectors);
	int size = 0; 
	size = this->m* this->n;
	this->vectors = (float*)calloc(size, sizeof(float));
}
void EMATRIX::IDENTITY()
{
	int k = MIN(this->m,this->n);
	this->ZEROS();
	for (int i = 0; i < k; i++)
		this->vectors[i + i * this->n] = 1;
}
void EMATRIX::ONES() 
{
	int size = this->m * this->n;
	for (int i = 0; i < size; i++)
	{
		this->vectors[i] = 1;
	}
}
EMATRIX* Least_Square_Solution(EMATRIX* A, EMATRIX* B)
//A(m,n) and B(n,p) must be required
{
	EMATRIX* AT = A->T();
	EMATRIX* ATA = (*AT) * (*A);
	EMATRIX* ATB = (*AT) * (*B);
	EMATRIX* AB = MATRIXCOMBINE(ATA, ATB);
	EMATRIX* G = Guassain_elimination(AB, AB->n - ATB->n);
	G->COLUMCUT(AB->n -ATB->n , AB->n - 1);
	free(AT);
	free(ATA);
	free(ATB);
	free(AB);
	return G;
}
EMATRIX* MATRIXCOMBINE(EMATRIX* A, EMATRIX* B)
{
	if (A->m != B->m)
	{
		cout << "ERROR! A->m and B->m is not equal" << endl;
		return nullptr;
	}
	else
	{
		int size_AlB = ((A->m) * (A->n) + (B->m) * (B->n));
		int AlB_m = A->m;
		int AlB_n = A->n + B->n;
		EMATRIX* AlB = (EMATRIX*)malloc(sizeof(EMATRIX));
		AlB->m = AlB_m;
		AlB->n = AlB_n;
		AlB->vectors = (float*)malloc(sizeof(float) * size_AlB);
		for (int i = 0; i < size_AlB; i++)
		{
			int m = i / AlB_n;
			int n = i % AlB_n;
			if (n < A->n)
				AlB->vectors[i] = A->vectors[n + m * A->n];
			else
			{
				int Bn = n - A->n;
				AlB->vectors[i] = B->vectors[Bn + m * B->n];
			}
		}
		return AlB;
	}
}
EMATRIX* Guassain_elimination(EMATRIX* A, int limit) {
	//Initializing
	EMATRIX* G = (EMATRIX*)malloc(sizeof(EMATRIX));
	G->m = A->m;
	G->n = A->n;
	int size_G = G->m * G->n;
	G->vectors = (float*)malloc(sizeof(float) * size_G);
	memmove(G->vectors, A->vectors, sizeof(float) * size_G);

	int n = 0;
	int m = 0;
	for (int n = 0; n < limit; n++)
	{
		float current = G->vectors[n + m * G->n];
		//If it is Zero
		if (current == 0)
			for (int k = 0; k < G->m; k++)
				if (G->vectors[n + k * G->n] != 0)
					swap_rows(G, k, m);
		//IF the column has at least one nonzero rows.
		if (current != 0)
		{
			//if it is no 1
			if (current != 1)
				Div_rows(G, m, G->vectors[n + m * G->n]);
			//Seaching other nonzero in the same column
			for (int k = 0; k < G->m; k++)
				if (m != k && G->vectors[n + k * G->n] != 0)
					Multi_Add_rows(G, k, m, -G->vectors[n + k * G->n]);
			//increase the row 
			//Since in Linear Algebra # pivot < # colum
			m++;
		}
	}
	return G;
}
EMATRIX* PROJECTION(EMATRIX* A, EMATRIX* B)
{
	EMATRIX* AT = A->T();
	EMATRIX* ATA = (*AT) * (*A);
	EMATRIX* ATB = (*AT) * (*B);
	EMATRIX* AB = MATRIXCOMBINE(ATA, ATB);
	EMATRIX* G = Guassain_elimination(AB, AB->n - ATB->n);
	G->COLUMCUT(AB->n - ATB->n, AB->n - 1);
	EMATRIX* P = (*A) * (*G);
	free(AT);
	free(ATA);
	free(ATB);
	free(AB);
	free(G);
	return P;
}
//Row Operator
void swap_rows(EMATRIX* A, int r1, int r2)
{
	if(r1 != r2)
		for (int k = 0; k < A->n; k++)
		{
			float temp = A->vectors[k + r1 * A->n];
			A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n];
			A->vectors[k + r2 * A->n] = temp;
		}
}
void Add_rows(EMATRIX* A, int r1, int r2)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n];
	}
}
void Sub_rows(EMATRIX* A, int r1, int r2)//subtract R1- R2
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] =A->vectors[k + r1 * A->n] - A->vectors[k + r2 * A->n];
	}
}
void Multi_rows(EMATRIX* A, int r1, int r2)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n] * A->vectors[k + r1 * A->n];
	}
}
void Div_rows(EMATRIX* A, int r1, float num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n]/num;
	}
}
void Multi_Add_rows(EMATRIX* A, int r1, int r2, float num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n]*num;
	}
}
//calculation
int MAX(int A, int B) { if (A > B) return A; return B; }
int MIN(int A, int B) { if (A > B) return B; return A; }


