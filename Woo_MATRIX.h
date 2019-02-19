#pragma once
#include <stdio.h>
#include <string.h>

struct MATRIXF
{
private:
	int row=0, col=0;
	float* matrix= NULL;
public:	
	MATRIXF(const MATRIXF &me);	
	~MATRIXF(void);
	MATRIXF(int r, int c);
	void MATRIX_SHOW(void);
	float* MATRIX_POP(void);
	void operator = (MATRIXF &M);
	void operator = (float* m);
	MATRIXF operator + (MATRIXF &M);
	MATRIXF operator *(MATRIXF &B);
};