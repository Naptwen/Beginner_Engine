#include "Window_rendering.h"

void WindowWH(int* w, int* h) 
{
	RECT rect;
	HWND myconsole = GetConsoleWindow();
	HDC hdc = GetDC(myconsole);
	GetWindowRect(myconsole, &rect);

	*w = rect.right - rect.left;
	*h = rect.bottom - rect.top;
}
//str2wstr
wstring s2w(string str)
{
	wstring stemp = wstring(str.begin(), str.end());
	copy(str.begin(), str.end(), stemp.begin());
	return stemp;
}
//x and y are the position of pixel
//Draw Bitmap
void DrawBitBlt(int x, int y, string name)
{
	HWND myconsole = GetConsoleWindow();
	HDC hdc = GetDC(myconsole);
	wstring wstr = s2w(name);
	LPCWSTR str = wstr.c_str();
	HBITMAP hBit = (HBITMAP)LoadImage(NULL, str,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	ReleaseDC(myconsole, hdc);
	DeleteDC(MemDC);
}
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
