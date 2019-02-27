#pragma once
#include "Woo_MATRIX.h"
#include "Woo_malloc.h"

MATRIXF::MATRIXF(const MATRIXF &me)
{
	if (this->row != 0 || this->col != 0)
	{
		woolloc_free(this->matrix);
	}
	this->row = me.row;
	this->col = me.col;
	this->matrix = (float*)woolloc(sizeof(float)*this->row*this->col);
	woolloc_copy(me.matrix, this->matrix, sizeof(float)*this->row*this->col);
	//printf("[MATRIXF] : MATRIX<%p> deepcopy MATRIX <%p> \n", this->matrix, me.matrix);
}
MATRIXF::MATRIXF(int r, int c)
{
	if (r == 0 || c == 0)
	{
		//printf("[MATRIX] : ERROR NO ROW OR COL VALUE\n");
	}
	else
	{
		row = r;
		col = c;

		this->matrix = (float*)woolloc(sizeof(float) *this->row*this->col);
		//printf("[MATRIXF] MATRIX CREAT <%p>\n", this->matrix);
		//printf("[MATRIX] : %d X %d MATRIX COMPLETED\n", this->row, this->col);
	}

}
MATRIXF::~MATRIXF(void)
{
	//printf("[MATRIXF] MATRIX DELETE <%p>\n", this->matrix);
	woolloc_free(this->matrix);
	this->matrix = NULL;
}
void MATRIXF::MATRIX_SHOW(void)
{
	//printf("[MATRIXF] MATRIX SHOW <%p> [%d]|[%d] \n",this->matrix, this->row, this->col);
	for (int i = 0; i < row; i++)
	{
		//printf("\n");
		for (int j = 0; j < col; j++)
		{
			//printf(" [%d]([%d][%d])=%f ", i*this->col+j, i, j, this->matrix[j + i*this->col]);
		}
	}
	//printf("\n\n");
}
float* MATRIXF::MATRIX_POP(void)
{
	return this->matrix;
}
void MATRIXF::operator = (MATRIXF &M)
{
	//printf("[MATRIXF] : MATRIX<%p> = MATRIX <%p>\n", this->matrix, M.matrix);
	if (this->row != 0 || this->col != 0)
	{
		woolloc_free(this->matrix);
	}
	this->row = M.row;
	this->col = M.col;
	this->matrix = (float*)woolloc(sizeof(float)*this->row*this->col);
	woolloc_copy(M.matrix, this->matrix, sizeof(float)*this->row*this->col);
}
void MATRIXF::operator = (float* m)
{
	if (m[this->col*this->row - 1] == NULL)
	{
		//printf("[MATRIXF] : THE PUSH MATRIX SIZE IS NOT MATCHING WITH THE SIZE OF MAATRIXF \n");
	}
	else
	{
		//printf("[MATRIXF] : float array into MATRIX <%p>\n", this->matrix);
		woolloc_free(this->matrix);
		this->matrix = (float*)woolloc(sizeof(float)*this->row*this->col);
		woolloc_copy(m, this->matrix, sizeof(float)*this->row*this->col);
	}
}
MATRIXF MATRIXF::operator + (MATRIXF &M)
{
	MATRIXF C(this->row, this->col);
	if (this->col*this->row != M.row*M.col)
	{
		//printf("[MATRIXF] : THE PUSH MATRIX SIZE IS NOT MATCHING WITH THE SIZE OF MAATRIXF \n");
	}
	else {
		//printf("[MATRIXF] : MATRIX<%p> + MATRIX <%p>\n", this->matrix, M.matrix);
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				C.matrix[i*this->col + j] = this->matrix[i*this->col + j] + M.matrix[i*this->col + j];
			}
		}
	}
	C.MATRIX_SHOW();
	return C;
}
MATRIXF MATRIXF::operator *(MATRIXF &B)
{
	if (this->col != B.row)
	{
		//printf("[ERROR] MATRIX A COL and MATRIX B ROW IS NOT MATCHING!\n");
	}
	else
	{
		//printf("[MATRIXF] : MATRIX<%p> * MATRIX <%p>\n", this->matrix, B.matrix);
		int r = this->row;
		int h = B.row;
		int w = B.col;
		MATRIXF C(r, w);
		int i, j, k;
		for (i = 0; i < r; i++)
		{
			for (j = 0; j < w; j++)
			{
				for (k = 0; k < h; k++)
				{
					//printf("(%d)[%d][%d] = [%d] [%d]\n",j+i*w,i,j, k + i*h, j + k*w);
					//printf("(%d)[%d][%d] = %f * %f\n", j + i*w, i, j, this->matrix[k + i*h], B.matrix[j + k*w]);
					C.matrix[j + i*w] += this->matrix[k + i*h] * B.matrix[j + k*w];
				}
			}
		}
		//printf("[MATRIXFX] COMPLETED\n");
		return C;
	}
}
