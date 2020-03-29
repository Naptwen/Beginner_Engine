#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ELOG.h"
using namespace std;

//Reference the wikipidea Bitmap information
#pragma pack(2) //for making byte 2
typedef struct {
	unsigned short bhmagic = 0;
	unsigned int   bhsize = 0;
	unsigned short bhrev1 = 0;
	unsigned short bhrev2 = 0;
	unsigned int   bhoffset = 0;
} BMPHEADER;
#pragma pack()
typedef struct {
	unsigned int   biSize = 0;
	unsigned int   biWidth = 0;
	unsigned int   biHeight = 0;
	unsigned short biPlanes = 0;
	unsigned short biBitCount = 0;
	unsigned int   biCompression = 0;
	unsigned int   biSizeImage = 0;
	unsigned int   biXPelsPerMeter = 0;
	unsigned int   biYPelsPerMeter = 0;
	unsigned int   biClrUsed = 0;
	unsigned int   biClrImportant = 0;
} BMPINFO;
//the x y position is based on lower left corner of bmp
typedef struct {
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char rev = 0;
	int x = 0;
	int y = 0;
}BMPRGB;
class EBMP {
public:
	BMPHEADER bh;
	BMPINFO bi;
	BMPRGB* brgb;
	string file_name;
	int Gap;
	int size;
	void EBMP::INFORMATION();
	void EBMP::ROTATION(int degree);
	void EBMP::CUT(int x, int y, int w, int h);
	void EBMP::RESET();
	EBMP::EBMP(string file_name);
	EBMP::EBMP();
	EBMP::~EBMP();
private:
	void EBMP::LOADBMP();
};