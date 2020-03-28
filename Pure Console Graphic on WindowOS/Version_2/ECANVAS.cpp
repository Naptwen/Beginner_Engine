#pragma once
#include "ECANVAS.h"	
//MATRIX

//start from lower left corner to right upper corner
CANVAS::CANVAS(int w, int h)
{
	this->w = w;
	this->h = h;
	this->pixels = (unsigned int*)calloc( w * h, sizeof(unsigned int) );
}
CANVAS::~CANVAS() {
	if(this->pixels != NULL)
		free(pixels);
	pixels = NULL;
}

void CANVAS::CLEARCANVAS() 
{
	if (pixels != NULL)
		free(pixels);
	pixels = (unsigned int*)calloc(w * h, sizeof(unsigned int));
}
void CANVAS::PAINTPOINIT(int x, int y, COLORCODE* color)
{
	int temp;
	int num;
	if (y >= 0 && y < this->h && x >= 0 && x < this->w)
	{
		temp = color->EXTRACTION();
		num = x + y * w;
		this->pixels[num] = temp;
	}
}
void CANVAS::PAINTLINE(int x1, int y1, int x2, int y2, COLORCODE* color)
{
	int temp = color->EXTRACTION();
	int dx = x2 - x1;
	int dy = y2 - y1;
	int j = 0;
	if (dx == 0)
	{
		if (y1 > y2)
			swap(y1, y2);
		for (int i = y1; i < y2; i++)
		{
			if (x1 < this->w && i < this->h)
				this->pixels[x1 + i * this->w] = temp;
		}
	}
	else if (dy == 0)
	{
		if (x1 > x2)
			swap(x1, x2);
		for (int i = x1; i < x2; i++)
		{
			if (i < this->w && y1 < this->h)
				this->pixels[i + y1 * this->w] = temp;
		}
	}
	else 
	{
		if (x1 > x2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		for (int i = x1; i < x2; i++)
		{
			j = y1 + dy * (i - x1) / dx;
			if (i < this->w && j < this->h)
				this->pixels[i + j * this->w] = temp;
		}
	}
}

void CANVAS::c2bit24(string name)
{
	int pixel_byte = 24 / 8;
	int gap = 4 - (pixel_byte * this->w) % 4;
	if (gap == 4)
		gap = 0;
	int total_size = 54 + (pixel_byte * this->w + gap) * this->h;
	const char* temp = (const char*)malloc(sizeof(char)* total_size);
	int length = w * h;
	//Add Bitmap header
	BMPHEADER bh;
	bh.bhmagic = 0x4d42;
	bh.bhsize = total_size;
	bh.bhrev1 = 0;
	bh.bhrev2 = 0;
	bh.bhoffset = 54;
	BMPINFO bi;
	bi.biSize = 40;
	bi.biWidth = w;
	bi.biHeight = h;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = 0;
	bi.biSizeImage = total_size - 54;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	
	std::memmove((void*)temp, &bh, sizeof(BMPHEADER));
	std::memmove((void*)(temp + 14), &bi, sizeof(BMPINFO));

	int j = 54; // from the Bitmap color info
	int k = 0; // for filling the gap
	for (int i = 0; i < length; i++)
	{
		const unsigned char r = (*(int*)((void*)(this->pixels + i)) & 0xff000000) >> 24;
		const unsigned char g = (*(int*)((void*)(this->pixels + i)) & 0x00ff0000) >> 16;
		const unsigned char b = (*(int*)((void*)(this->pixels + i)) & 0x0000ff00) >> 8;
		memset((void*)(temp + j), (const char)b, sizeof(char));
		memset((void*)(temp + j + 1), (const char)g, sizeof(char));
		memset((void*)(temp + j + 2), (const char)r, sizeof(char));
		j += pixel_byte;
		k++;
		if (k == this->w)
		{
			for (int g = 0; g < gap; g++) 
				memset((void*)(temp + j + g), (const char)0, sizeof(char));
			j += gap;
			k = 0;
		}
	}
	//BMP SAVE
	ofstream ofile(name, ios::out | ios::binary);
	ofile.write((char*)temp, total_size);
	ofile.close();
	free((void*)temp);
}
void CANVAS::c2txt(string name)
{
	int length = this->w * this->h;
	int k = 0;
	string out_line;
	ofstream out(name);
	for (int i = 0; i < length; i++)
	{
		out << hex<< this->pixels[i] <<", ";
		k++;
		if (k == this->w)
		{
			out << endl;
			k = 0;
		}
	}
	out.close();
}

void CANVAS::PushMATRIX(EMATRIX* A) 
{
	int m = A->m;
	int n = A->n;
	int length = m * n;
	int px = 0, py = 0;
	float* vec = A->vectors;
	for (int i = 0; i < length; i++)
	{
		//float and int are the same byte
		const unsigned char r = (*(int*)((void*)(vec + i)) & 0xFF000000) >> 24;
		const unsigned char g = (*(int*)((void*)(vec + i)) & 0xFF000000) >> 16;
		const unsigned char b = (*(int*)((void*)(vec + i)) & 0xFF000000) >> 8;
		COLORCODE color(r, g, b);
		px = i / n;
		py = i % n;
		if (!(color == MAGENTA))
			PAINTPOINIT(px, py, &color);
	}
}
void CANVAS::PushEBMP(EBMP* ebmp, int x, int y, int pixel_size)
{
	int k = 0;
	unsigned char R = 0, G = 0, B = 0;
	int h = ebmp->bi.biHeight;
	int w = ebmp->bi.biWidth;
	int length = h * w; // # of pixels in BMP
	for (int k = 0; k < length; k++) //
	{
		B = ebmp->brgb[k].b;
		G = ebmp->brgb[k].g;
		R = ebmp->brgb[k].r;
		int px = ebmp->brgb[k].x + x;
		int py = ebmp->brgb[k].y + y;
		COLORCODE color(R, G, B);
		if (!(color == MAGENTA))
			this->DrawPixel(pixel_size, px, py, &color);
	}
}
void CANVAS::DrawPixel(int pixel_size, int x, int y, COLORCODE* color)
{
	int pos_x = x * (pixel_size); // As multiplying 2 we can make a interval
	int pos_y = y * (pixel_size); //Swap the y poisition to start from lower left corner
	for (int j = 0; j < pixel_size; j++)
		for (int i = 0; i < pixel_size; i++)
		{
			this->PAINTPOINIT(pos_x + i, pos_y + j, color);
		}
}
//FOR MAKING COLOR instead of using int[3]
void COLORCODE::operator = (unsigned int value)
{
	this->R = value >> 24;
	this->G = value >> 16;
	this->B = value >> 8;
}
COLORCODE::COLORCODE(unsigned char R, unsigned char G, unsigned char B) {
	this->R = R;
	this->G = G;
	this->B = B;
}
unsigned int COLORCODE::EXTRACTION() {
	int temp = this->R;
	temp = temp << 8;
	temp += this->G;
	temp = temp << 8;
	temp += this->B;
	temp = temp << 8;
	return temp;
}
bool COLORCODE::operator == (unsigned int value)
{
	unsigned char r = value >> 24;
	unsigned char g = value >> 16;
	unsigned char b = value >> 8;
	if (this->R == r)
		if (this->G == g)
			if (this->B == b)
				return true;
	return false;
}
