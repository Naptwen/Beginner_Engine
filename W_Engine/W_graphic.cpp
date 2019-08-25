#include "W_graphic.h"
frame::frame(int w, int h, int pos_x, int pos_y)
{
	HWND myconsole = GetConsoleWindow();
	RECT r;
	GetWindowRect(myconsole, &r);
	this->w = w;
	this->h = h; 
	this->o_x = pos_x;
	this->o_y = h+ pos_y;
}
void Console_draw_line(int R, int G, int B, vector2 const &A, vector2 const &C, frame const &f)
{
	HWND myconsole = GetConsoleWindow();
	HDC mydc = GetDC(myconsole);
	COLORREF COLOR = RGB(R, G, B);
	if (A.x == C.x)
	{
		int max = max(A.y, C.y);
		int min = min(A.y, C.y);
		for (int j = min; j <= max; j++)
		{
			int p_x = f.o_x + A.x;
			int p_y = f.o_y - j;
			SetPixel(mydc, p_x, p_y, COLOR);
		}
	}
	else if (A.y == C.y)
	{
		int max = max(A.x, C.x);
		int min = min(A.x, C.x);
		for (int i = min; i <= max; i++)
		{
			int p_x = f.o_x + i;
			int p_y = f.o_y - A.y;
			SetPixel(mydc, p_x, p_y, COLOR);
		}
	}
	float slope = ((float)C.y - (float)A.y) / ((float)C.x - (float)A.x);
	for (float l = A.x; l < C.x; l += 0.01f)
	{
		vector2 p_c_3 = { l, slope*(l - A.x) + A.y };
		int p_x = f.o_x + p_c_3.x;
		int p_y = f.o_y - p_c_3.y;
		SetPixel(mydc, p_x, p_y, COLOR);
	}
	ReleaseDC(myconsole, mydc);
}
void Console_Dot(int R, int G, int B, vector2 const p, frame const &taf)
{
	HWND myconsole = GetConsoleWindow();
	HDC mydc = GetDC(myconsole);
	COLORREF COLOR = RGB(R, G, B);
	int p_y = taf.o_y - p.y;
	int p_x = taf.o_x + p.x;
	SetPixel(mydc, p_x, p_y, COLOR);
	ReleaseDC(myconsole, mydc);
}
void Console_Circle(int R, int G, int B, vector2 const p, int r, frame const &taf) //drawing from center
{
	HWND myconsole = GetConsoleWindow();
	HDC mydc = GetDC(myconsole);
	COLORREF COLOR = RGB(R, G, B);

	for (int i = -r; i <= r; i++)
	{
		for (int j = -r; j <= r; j++)
		{
			double d = (i*i*3.14 + j * j*3.14) / (r*r);
			if (d <= 1) //if you change this condition, you can make a line circle
			{
				SetPixel(mydc, i + p.x + taf.o_x, taf.o_y - (j + p.y), COLOR);
			}
		}
	}
	ReleaseDC(myconsole, mydc);
}
void Console_Square(vector2 p, int r, frame &taf)//drawing from center
{
	HWND myconsole = GetConsoleWindow();
	HDC mydc = GetDC(myconsole);
	COLORREF COLOR = RGB(0, 255, 0);
	//x^2+y^2 = r^2 -> y^2 = r^2-x^2 -> y = sqrt(r^2-x^2) and -
	for (int i = p.x - r; i <= p.x + r; i++)
	{
		for (int j = p.y - r; j <= p.y + r; j++)
		{
			SetPixel(mydc, i + taf.o_x, taf.o_y - j, COLOR);
		}
	}
	ReleaseDC(myconsole, mydc);
}
