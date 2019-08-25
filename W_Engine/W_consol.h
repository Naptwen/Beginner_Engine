#pragma once
#include <Windows.h>
#include <string>
#include <conio.h>
#include <thread>
#include <bitset>
#include <vector>
#include "W_file.h"
#include "W_A_star.h"
#include "W_Physics.h"
#include "W_graphic.h"

struct Math_sym 
{
	float value;
	bool isNum;
};
//---------------------------------
void Char_split(char* &intext, char* &outtext, char split_cod); //directly change the value of pointer so we dont have to care about stack memory
int Char_size(char* &text);
void  MATH_TEXT_ANALAZING(char* &text);
void pointer_cleaner(char* &ptr); //it has some problem I will figure out but now I just let it
//-----------------------------------
void main_menu(void); 
void DATA_MENU(void); 
void MATH_MENU(void); 
void A_start_MENU(void);
void Machine_MENU(void);
void Physics_MENU(void);
///-------A*Algorithm-------------------------------------
void drawing(vector<Object>& const drawing_list, int R, int G, int B, float radius, frame & const f);
