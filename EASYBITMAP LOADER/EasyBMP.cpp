#include "EasyBMP.h"
//x = absolute position of origin of x, y = absolute position of origin of y
void Bitmap_reader(EBMP ebmp, int x, int y, int pixel_size)
{
	int k = 0;
	unsigned char R = 0, G = 0, B = 0;
	int h = ebmp.bi.biHeight;
	int w = ebmp.bi.biWidth;
	int length = h * w; // # of pixels in BMP
	for (int k = 0; k < length; k++) //
	{
		B = ebmp.brgb[k].b;
		G = ebmp.brgb[k].g;
		R = ebmp.brgb[k].r;
		int px = ebmp.brgb[k].x + x; // origin + the position of pixel
		int py = ebmp.brgb[k].y + y;
		COLORREF color = RGB(R, G, B);
		if (color == RGB(255, 0, 255)) // if it is magenta color, change its color to blakc
			color = RGB(0, 0, 0);
		DrawPixel(pixel_size, px, py, &color);
	}
}
//x and y are the position of pixel
void DrawPixel(int pixel_size, int x, int y, COLORREF* color)
{
	HWND myconsole = GetConsoleWindow();
	HDC mydc = GetDC(myconsole);
	RECT rect;
	GetClientRect(myconsole, &rect);
	int w = rect.right - rect.left; //get width of console window
	int h = rect.bottom - rect.top; //get height of console window
	int pix_w = pixel_size; //set size of width of pixel
	int pix_h = pixel_size; //set size of height of pixel

	if (pixel_size > 0) // if pixel size is bigger than 0
	{
		int pos_x = x * (pixel_size * 2); // As multiplying 2 we can make a interval
		int pos_y = h - y * (pixel_size * 2); //Swap the y poisition to start from lower left corner
		for (int j = -pix_h; j < pix_h; j++)
			for (int i = -pix_w; i < pix_w; i++)
			{
				SetPixel(mydc, pos_x + i, pos_y + j, *color);
			}
	}
	else //if pixel size is 0 it just a dot
	{
		int pos_x = x;
		int pos_y = h - y;
		SetPixel(mydc, pos_x, pos_y, *color);
	}
	ReleaseDC(myconsole, mydc);
}
void EBMP::BITMAP(string file_name)
{
	//Loading bmp file
	ifstream bmpfile;
	bmpfile.open(file_name, ios::in);
	if (bmpfile.good())
	{
		BMPHEADER tempheader;
		BMPINFO tempinfo;

		bmpfile.read((char*)&tempheader, sizeof(BMPHEADER)); // Read 14 byte of bmp information
		bmpfile.read((char*)&tempinfo, sizeof(BMPINFO));   // Read 40 byte of bmp information
		memcpy(&this->bh, &tempheader, sizeof(BMPHEADER)); // To move to heap memory
		memcpy(&this->bi, &tempinfo, sizeof(BMPINFO));     // To move to heap memory
		cout << sizeof(BMPHEADER) << " || " << sizeof(BMPINFO) << endl;
		int offset = this->bh.bhoffset;
		int bit = this->bi.biBitCount;
		int width = this->bi.biWidth;
		int height = this->bi.biHeight;
		int pixel_byte = bit / 8; //To get the byte, divide the bit by 8 
		int gap = 0;
		if (pixel_byte > 1 && pixel_byte != 4) //Gap is the memory that is not going to be used but for filling the empty area 
			gap = 4 - (pixel_byte * width) % 4; //We can get the size of gap by this formula since each byte is divieded by 4.
		unsigned int size = pixel_byte * width * height + height * gap; //It is the total size(byte) of information of color
		this->Gap = gap;
		this->size = size;

		unsigned char* tempdata = new unsigned char[size];
		bmpfile.seekg(offset, ios::beg); //It is not neccesary 
		bmpfile.read((char*)tempdata, size * sizeof(unsigned char)); //read the information of pixel color
		int i = 0, j = 0, w = 0, h =0; 
		int length = width * height; //The totla # of pixel
		this->brgb = new BMPRGB[length];
		while (i < length)
		{
			this->brgb[i].r = tempdata[j + 2];
			this->brgb[i].g = tempdata[j + 1];
			this->brgb[i].b = tempdata[j];
			this->brgb[i].x = w;
			this->brgb[i].y = h;
			w++;
			i++;
			j += 3;
			if (w == width) //Since every gap is placed in the last part of the width
			{
				j += gap;  //Add gap to the index of j to skip the gap memory
				w = 0;     //To initialize the position of x
				h++;	  //increase the poisiton of y
			}
		}
		delete[] tempdata;
	}
	else
		cout << "Reading Error" << endl;
	bmpfile.close();
}
EBMP::EBMP() 
{
	//Initializing
	BMPHEADER ht;
	BMPINFO hi;
	this->bh = ht;
	this->bi = hi;
	this->brgb = NULL;
	this->Gap = 0;
	this->size = 0;
}
EBMP::~EBMP() {
	delete[] this->brgb;
}