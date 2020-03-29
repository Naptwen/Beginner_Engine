#include "EOPEN.h"
#include <conio.h>

CANVAS* OPENCLSET(string title, int w, int h, int cw, int ch, char OS, const char TYPE)
{
	SETCONSOLESIZE(title, w, h, OS, TYPE);
	if (cw == -1)
		cw = w;
	if (cw == -1)
		ch = h;
	return SETCANVASSIZE(w, h);
}
void SETCONSOLESIZE(string title, int w, int h, char OS, const char TYPE)
{
	switch (OS)
	{
	case WINDOW:
	{
		HWND console = GetConsoleWindow();
		RECT r;
		string tt = "Title : " + title;
		system(tt.c_str());
		GetWindowRect(console, &r);
		MoveWindow(console, 50, 50, 640, 480, SWP_SHOWWINDOW | SWP_NOMOVE);

		if (TYPE == BORDERLESS)
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo(handle, &info);
			COORD new_size =
			{
				info.srWindow.Right - info.srWindow.Left + 1,
				info.srWindow.Bottom - info.srWindow.Top + 1
			};
			SetConsoleScreenBufferSize(handle, new_size);

			ReleaseDC(console, NULL);
		}
		break;
	}
	default:
		break;
	}
}
CANVAS* SETCANVASSIZE(int w, int h)
{
	CANVAS* canvas = (CANVAS*)malloc(sizeof(CANVAS));
	CANVAS temp(w, h);
	memcpy(canvas, &temp, sizeof(CANVAS));
	canvas->pixels = (unsigned int*)malloc(sizeof(int) * canvas->w * canvas->h);
	memcpy(canvas->pixels, temp.pixels, sizeof(int) * canvas->w * canvas->h);
	return canvas;
}

EMATRIX* SETCAMERA(float fx, float fy, float cx, float cy, float rx, float ry, float rz, float tx, float ty, float tz)
{
	float camera_Matrix[] =
	{
		fx,0,cx, cos(rx) * cos(ry), cos(rx) * sin(ry) * sin(rz) - sin(rx) * cos(rz), cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz),tx,
		0,fy,cy, sin(rx) * cos(ry), sin(rx) * sin(ry) * sin(rz) + cos(rx) * cos(rz), sin(rx) * sin(ry) * cos(rz) - cos(rx) * sin(rz),ty,
		0, 0, 1, -sin(ry), cos(ry) * sin(rz), cos(ry) * cos(rz),tz
	};
	EMATRIX* CAMERA_ptr = (EMATRIX*)malloc(sizeof(EMATRIX));
	CAMERA_ptr->m = 3;
	CAMERA_ptr->n = 7;
	CAMERA_ptr->vectors = (float*)malloc(sizeof(float) * 21);
	memmove(CAMERA_ptr->vectors, camera_Matrix, sizeof(float) * 21);
	return CAMERA_ptr;
}
EMATRIX* CAMERA_MATRIX(EMATRIX CAMERA, EMATRIX A)

{
	EMATRIX* AT = A.T();
	EMATRIX I(1, AT->n); I.ONES();
	EMATRIX* AT_I = MATRIXADDROW(AT, &I);
	EMATRIX INTRINSICMATRIX;
	INTRINSICMATRIX = CAMERA;
	INTRINSICMATRIX.COLUMCUT(0, 2);
	EMATRIX EXTRINSICCMATRIX;
	EXTRINSICCMATRIX = CAMERA;
	EXTRINSICCMATRIX.COLUMCUT(3, 6);
	EMATRIX* B = INTRINSICMATRIX * EXTRINSICCMATRIX;
	EMATRIX* C = (*B) * (*AT_I);
	C = C->T();
	C->COLUMCUT(0, 1);
	free(B);
	free(AT);
	free(AT_I);
	return C;

}

void OPENCL_BMP(CANVAS* canvas, string file_name, int x, int y, int pixel)
{
	elog << "OPEN BMP FILE : " + file_name + "\n";
	EBMP ebmp(file_name);
	if (&ebmp != NULL)
		canvas->PushEBMP(&ebmp, x, y, pixel);
}
void OPENCL_MATRIX(CANVAS* canvas, EMATRIX A, COLORCODE b, const char TYPE)
{
	void (CANVAS:: * temp)(EMATRIX*); //function pointer 
	temp = nullptr;
	if (A.n != 2)
	{
		elog << ("ERROR : A MUST BE 2 column\n");
	}
	else
	{
		switch (TYPE)
		{
		case PIXEL:
			temp = &CANVAS::PushMATRIXP;
			break;
		case LINE:
			temp = &CANVAS::PushMATRIXL;
			break;
		}
		EMATRIX C(A.m, 1);
		C.ONES();
		EMATRIX* D = C * (float)b.EXTRACTION();
		EMATRIX* B = MATRIXADDCOL(&A, D);
		(canvas->*temp)(B);
		free(D);
		free(B);
	}
}

void GRAPHICDISPLAYER(CANVAS& canvas)
{
	canvas.c2bit24("./CANVAS.bmp");
	DrawBitBlt(0, 0, "./CANVAS.BMP");
}
void GRAPHICDISCLEAR(CANVAS& canvas)
{
	canvas.CLEARCANVAS();
}









