#include "EBMP.h"
void EBMP::ROTATION(int degree)
{
	int bit_w = this->bi.biWidth;
	int bit_h = this->bi.biHeight;
	int x = bit_w;
	int y = bit_h;
	double radian = (double)degree * 180 / PI;
	for (int j = 0; j < bit_h; j++)
		for (int i = 0; i < bit_w; i++)
		{
			int n = i + j * bit_w;
			int rx = (this->brgb[n].x * cos(radian) - this->brgb[n].y * sin(radian));
			int ry = (this->brgb[n].x * sin(radian) + this->brgb[n].y * cos(radian));
			this->brgb[n].x = rx;
			this->brgb[n].y = ry;
			//cout << rx << "  || " << ry << endl;
		}
}
void EBMP::CUT(int x, int y, int w, int h)
{
	int bit_w = this->bi.biWidth;
	int bit_h = this->bi.biHeight;
	for (int j = 0; j < bit_h; j++)
		for (int i = 0; i < bit_w; i++)
		{
			if ((i< x || i >w) || (j < y || j >h))
			{
				int n = i + j * bit_w;
				this->brgb[n].b = 255;
				this->brgb[n].g = 0;
				this->brgb[n].r = 255;
			}
		}
}
void EBMP::RESET()
{
	LOADBMP();
}
EBMP::EBMP(string file_name)
{
	//Initializing
	BMPHEADER ht;
	BMPINFO hi;
	this->bh = ht;
	this->bi = hi;
	this->brgb = NULL;
	this->Gap = 0;
	this->size = 0;
	this->file_name = file_name;
	//Loading bmp file
	LOADBMP();
}
void EBMP::LOADBMP()
{
	string file_name = this->file_name;
	ifstream bmpfile;
	bmpfile.open(file_name, ios::in);
	if (bmpfile.good())
	{
		bmpfile.seekg(0, ios::end);
		unsigned int file_size = bmpfile.tellg();
		bmpfile.seekg(0, ios::beg);
		char* data = (char*)calloc(file_size, sizeof(char));
		if (!bmpfile.read((char*)data, file_size))
			std::cerr << " failed to read from file" << endl;

		BMPHEADER tempheader;
		BMPINFO tempinfo;
		std::memmove(&this->bh, (void*)(data), sizeof(BMPHEADER)); // To move to heap memory
		std::memmove(&this->bi, (void*)(data + 14), sizeof(BMPINFO));     // To move to heap memory
		unsigned int pixel_size = file_size - 54;

		int bit = this->bi.biBitCount;
		int width = this->bi.biWidth;
		int height = this->bi.biHeight;
		int pixel_byte = bit / 8; //To get the byte, divide the bit by 8 
		int gap = 4 - (pixel_byte * width) % 4;
		if (gap == 4)
			gap = 0; //It is the total size(byte) of information of color
		this->Gap = gap;
		this->size = size;
		int i = 0, j = 54, w = 0, h = 0;
		int length = width * height; //The totla # of pixel
		this->brgb = new BMPRGB[length];
		while (i < length)
		{
			this->brgb[i].r = *(char*)((void*)(data + j + 2));
			this->brgb[i].g = *(char*)((void*)(data + j + 1));
			this->brgb[i].b = *(char*)((void*)(data + j));
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
		free((void*)data);
	}
	else
		std::cerr << " failed to open file" << endl;
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
void EBMP::INFORMATION()
{
	std::cout << "---------BIT infromation-----------" << endl;
	std::cout << " mgic : " << bh.bhmagic << endl;
	std::cout << " size : " << bh.bhsize << endl;
	std::cout << " rev1 : " << bh.bhrev1 << endl;
	std::cout << " rev2 : " << bh.bhrev2 << endl;
	std::cout << " offset : " << bh.bhoffset << endl;
	std::cout << " hdsize : " << bi.biSize << endl;
	std::cout << " width : " << bi.biWidth << endl;
	std::cout << " height : " << bi.biHeight << endl;
	std::cout << " plan : " << bi.biPlanes << endl;
	std::cout << " #bit : " << bi.biBitCount << endl;
	std::cout << " comp : " << bi.biCompression << endl;
	std::cout << " Isize : " << bi.biSizeImage << endl;
	std::cout << " dpix : " << bi.biXPelsPerMeter << endl;
	std::cout << " dpiy : " << bi.biYPelsPerMeter << endl;
	std::cout << " color : " << bi.biClrUsed << endl;
	std::cout << " imcolor : " << bi.biClrImportant << endl;
	std::cout << "--------------------------------------" << endl;

}
