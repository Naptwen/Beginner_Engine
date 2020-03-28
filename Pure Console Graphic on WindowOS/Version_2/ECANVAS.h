#pragma once
#include "EBMP.h"
#include "EMATRIX.h"

#define MAGENTA 0xFF00FF00
#define PI 3.141592
class COLORCODE {
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;
	void COLORCODE::operator = (unsigned int value);
	COLORCODE::COLORCODE(unsigned char R, unsigned char G, unsigned char B);
	unsigned int COLORCODE::EXTRACTION();
	bool COLORCODE::operator == (unsigned int value);
};
class CANVAS 
{
public:
	unsigned int* pixels;
	CANVAS::CANVAS(int w, int h);
	CANVAS::~CANVAS();
	void CANVAS::CLEARCANVAS();
	void CANVAS::PAINTPOINIT(int x, int y, COLORCODE* color);
	void CANVAS::PAINTLINE(int x1, int y1, int x2, int y2, COLORCODE* color);

	void CANVAS::c2txt(string name);
	void CANVAS::c2bit24(string name);

	void CANVAS::PushMATRIX(EMATRIX* A);
	void CANVAS::PushEBMP(EBMP* ebmp, int x, int y, int pixel_size);
	void CANVAS::DrawPixel(int pixel_size, int x, int y, COLORCODE* color);
private : 
	unsigned int w;
	unsigned int h;
};

