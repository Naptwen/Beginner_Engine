#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std;
class EMATRIX{
private:
	int* vectors;
	int m; //row
	int n; //column
public:
	EMATRIX::EMATRIX(int m, int n);
	EMATRIX::~EMATRIX();
	void EMATRIX::operator = (int* matrix);
	void EMATRIX::operator = (EMATRIX matrix);
	void EMATRIX::operator * (EMATRIX B);
	void EMATRIX::T();
	void EMATRIX::info();
	void EMATRIX::m2txt(string name);
	int* EMATRIX::DIMENSION();
	int* EMATRIX::VECTORS();
};
