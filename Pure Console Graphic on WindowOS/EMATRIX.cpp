#include "EMATRIX.h"
EMATRIX::EMATRIX(int m, int n)
{
	this->m = m;
	this->n = n;
	vectors = (int*)malloc(sizeof(int) * m * n);
}
EMATRIX::~EMATRIX()
{
	if (vectors != NULL)
		free(vectors);
	vectors = NULL;
}
void EMATRIX::operator = (int* matrix)
{
	memmove(vectors, matrix, m * n * sizeof(int));
}
void EMATRIX::operator = (EMATRIX matrix)
{
	if (this->vectors != NULL)
		free((void*)vectors);
	this->vectors = (int*)calloc(matrix.m * matrix.n, sizeof(int));
	memmove((void*)(this->vectors), matrix.vectors, m * n * sizeof(int));
	this->m = matrix.m;
	this->n = matrix.n;
}
void EMATRIX::operator * (EMATRIX B)
{
	if (this->n == B.m)
	{
		//A = m x n and B = n x p So C=AB is  m x p
		int cm = this->m;
		int cn = B.n;
		int num = cm * cn;
		int* transition = (int*)calloc(num, sizeof(int));
		int x = 0, y = 0, k = 0;
		for (int i = 0; i < num; i++)
		{
			x = i / cn; //Why it is different with the coordinate position of vecotr2D on screen?
			y = i % cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
			int sum = 0;//														     (1,0)
			for (int k = 0; k < this->n; k++) // from 0 to n
			{
				int index_A = k + x * this->n; //Axk
				int index_B = y + k * B.n;     //Bky
				int temp = this->vectors[index_A] * B.vectors[index_B];

				sum += temp;
			}
			transition[i] = sum; //Cxy = Sigma (Axk * Bky)
			k++;
		}
		if (this->vectors != NULL)
			free((void*)this->vectors);
		this->vectors = (int*)calloc(num, sizeof(int));
		memmove((void*)vectors, (void*)transition, sizeof(int) * num);
		this->m = cm;
		this->n = cn;
		free(transition);
	}
	else
		cout << "ERROR : MATRIX SIZE IS NOT SAME" << endl;
}
void EMATRIX::T()
{
	int num = m * n;
	int x = 0, y = 0, temp_m = 0, swapx = 0, swapy = 0, j = 0, k = 0;
	if (m < n)
		temp_m = n;
	else
		temp_m = m;
	int length_temp = temp_m * temp_m; //TO make a n x n matrix for calculate

	int* transition = (int*)calloc(num, sizeof(int));
	for (int i = 0; i < length_temp; i++)
	{
		x = i / n;
		y = i % n;
		j = x + y * m; //reverse
		if (k < num) // if it filled all the capacity we don't need to do more.
		{
			transition[j] = vectors[y + x * n];
			k++;
		}
	}
	int temp = m;
	m = n;
	n = temp;
	memmove((void*)vectors, (void*)transition, sizeof(int) * m * n);
	free(transition);
}
void EMATRIX::info()
{
	int length = this->m * this->n;
	int y = 1;
	for (int i = 0; i < length; i++)
	{
		cout << this->vectors[i] << ", ";
		if (y == this->n)
		{
			cout << "\n";
			y = 1;
		}
		else
			y++;
	}
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
int* EMATRIX::DIMENSION() {
	int* dimension = (int*)calloc(2, sizeof(int));
	dimension[0] = this->m;
	dimension[1] = this->n;
	return dimension;
}//Don't forget free(dimension)
int* EMATRIX::VECTORS() {
	int* returnvectors = (int*)malloc(sizeof(int) * m * n);
	returnvectors = this->vectors;
	return returnvectors;
}