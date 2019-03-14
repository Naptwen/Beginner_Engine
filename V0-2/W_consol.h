#pragma once
#include <Windows.h>
#include <string>
#include <conio.h>
#include <thread>
#include <bitset>
#include "W_file.h"

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
struct Math_sym 
{
	float value;
	bool isNum;
};
void Consol_Window(int x, int y);
void Consol_Color(int color);
void gotoxy(int x, int y);
void getxy(int &x, int &y);
void clearScreen(int sx, int sy, int ex, int ey);
//----------------------------------------------------
void USER_INPUT_EVENT_THREAD(void);
void USER_INPUT_EVENT_CONTROLLER(char* const key, int* const m_x, int* const m_y, int* m_key);
//---------------------------------
void Char_split(char* &intext, char* &outtext, char split_cod); //directly change the value of pointer so we dont have to care about stack memory
int Char_size(char* &text);
void  MATH_TEXT_ANALAZING(char* &text);
void pointer_cleaner(char* &ptr); //it has some problem I will figure out but now I just let it
//-----------------------------------
void main_menu(void); 
void DATA_MENU(void); 
void DRAW_MENU(void);
void PAINT_MENU(void);
void Thread_MENU(void);
void MATH_MENU(void);
//--------Thread--------
void PipeLine(int* const k, int* const result_k);
void PipeLineChecker(int* const a, int* const b, int* const c);
//-----------------------------------
void Consol_Line(int R, int G, int B, vector2 p1, vector2 p2, frame *taf);
void Consol_Dot(int R, int G, int B, vector2 p, frame *taf);
void Consol_Circle(vector2 p, int r, frame *taf); //using Area equation
void Consol_Square(vector2 p, int r, frame *taf);
