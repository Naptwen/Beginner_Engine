#pragma once
#include <conio.h>
#include <Windows.h>

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define TURQUOISE 3
#define RED 4
#define ORANGE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define BLIGHT 9
#define GLIGHT A //10
#define RLIGHT B //12
#define PLIGHT C //13
#define YLIGHT D //14
#define RED_28 255, 0, 0
#define GREEN_28 0, 255, 0
#define BLUE_28 0, 0, 255
#define WHITE_28 255, 255, 255
#define BLACK_28 0, 0, 0


class frame
{
public:
	int w, h;
	int o_x, o_y;
	frame(int w, int h, int pos_x, int pos_y);
};
class vector2
{
public:
	int x;
	int y;
	void operator = (int i[2]) //for easy way to declare x,y
	{
		this->x = i[0];
		this->y = i[1];
	}
};

void Console_draw_line(int R, int G, int B, vector2 const &A, vector2 const &C, frame const &f);
void Console_Dot(int R, int G, int B, vector2 const p, frame const &taf);
void Console_Circle(int R, int G, int B, vector2 const p, int r, frame const &taf);
void Console_Square(vector2 p, int r, frame &taf);
