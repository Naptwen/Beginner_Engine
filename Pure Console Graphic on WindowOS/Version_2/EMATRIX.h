#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;
class EMATRIX{
public:
	float* vectors;
	int m; //row
	int n; //column
	EMATRIX::EMATRIX();
	 EMATRIX::EMATRIX(int m, int n);
	 EMATRIX::~EMATRIX();
	 void EMATRIX::operator = (const float* matrix);
	 void EMATRIX::operator = (const EMATRIX &matrix);
	 EMATRIX* EMATRIX:: operator * (const EMATRIX &B);
	 EMATRIX* EMATRIX:: operator * (const float f);
	 EMATRIX* EMATRIX::operator + (const  float f);
	 EMATRIX* EMATRIX::T();
	 void EMATRIX::info();
	 void EMATRIX::m2txt(std::string name);
	 void EMATRIX::COLUMCUT(int st, int ed);
	 void EMATRIX::ROWCUT(int st, int ed);
	 void EMATRIX::ZEROS();
	 void EMATRIX::ONES();
	 void EMATRIX::IDENTITY();
};
int MAX(int A, int B);
int MIN(int A, int B);
void swap_rows(EMATRIX* A, int r1, int r2);
void Multi_rows(EMATRIX* A, int r1, float num);
void Add_rows(EMATRIX* A, int r1, int r2);
void Sub_rows(EMATRIX* A, int r1, int r2);
void Div_rows(EMATRIX* A, int r1, float num);
void Multi_Add_rows(EMATRIX* A, int r1, int r2, float num);
EMATRIX* Least_Square_Solution(EMATRIX* A, EMATRIX* B);
EMATRIX* MATRIXADDROW(EMATRIX* A, EMATRIX* B);
EMATRIX* MATRIXADDCOL(EMATRIX* A, EMATRIX* B);
EMATRIX* Guassain_elimination(EMATRIX* A, int limit); 
EMATRIX* PROJECTION(EMATRIX* A, EMATRIX* B);
