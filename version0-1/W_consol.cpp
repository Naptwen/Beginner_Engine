#pragma once
#include "W_consol.h"
//--------------------------------ETC function
void Consol_Window(int x, int y, string Title)
{
	string A = "mode con: cols=" + to_string(x) + " lines=" + to_string(y);
	system(A.c_str());
	system(Title.c_str());
}
void Consol_Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void gotoxy(int x, int y) //for moving cusor pos
{
	COORD Pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void getxy(int &x, int &y) //for gettting cursor current pos
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	x = info.dwCursorPosition.X;
	y = info.dwCursorPosition.Y;
}
void clearScreen(int sx, int sy, int ex, int ey) //for earsing specific area of texts
{
	gotoxy(sx, sy);
	for (int j = 0; j <= ey; j++)
	{
		for (int i = 0; i <= ex; i++)
		{
			std::cout << "\33[2K"; 
		}
		std::cout << endl;
	}
}
//-----------------------------------Dynamic char list controller
void Char_split(char* &intext , char* &outtext, char split_cod) // we will change the address directly and deep copying of array
{
	int temp_n = 0;
	int p = 0;
	int start_b = 0;
	int text_size = Char_size(intext); 
	if (text_size != 0)
	{
		while (true)
		{
			if (intext[p] == split_cod || intext[p] == NULL)
			{
				break;
			}
			else
			{
				p++;
			}
		}
		//-------output text-------
		pointer_cleaner(outtext);
		outtext = (char*)malloc(p + 1); //To put the NULL in the end of char array, we need to add one more value
		memcpy(outtext, intext, p); // !!Deep copy
		outtext[p] = '\0';
		//-------copy input text to temp-------	
		int remain_size = text_size - p - 1; //To put the NULL in the end of char array, we need to take one more value
		if (remain_size <= 0)
		{
			pointer_cleaner(intext);
			intext = (char*)malloc(1);
			intext[0] = '\0';
		}
		else
		{
			char* temp = (char*)malloc(remain_size + 1);
			memcpy(temp, intext, remain_size);
			temp[remain_size] = '\0';
			//-----------change input text----------
			pointer_cleaner(intext);
			intext = (char*)malloc(remain_size + 1);
			memcpy(intext, temp, remain_size); 
			intext[remain_size] = '\0';
			pointer_cleaner(temp); //the temp value is free in this function but other intext and outtext should be free, after using these at outside of fn 
		}
	}
}
int Char_size(char* &text)
{
	int i = 0;
	while (true)
	{
		if (text[i] == NULL)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	return i;
}
void pointer_cleaner(char* &ptr) //to clean up other elemental of array
{

	int i = 0;
	while (true)
	{
		if (ptr[i] == NULL)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	for (int k = 0; k <i; k++)
	{
		ptr[k] = 0;
	}
	free((void*)ptr[0]);
} //need to edit
//-----------------------------------ConSole UI function
void main_menu(void)
{
	char button;
	while (true)
	{
		SetConsoleTitle("BAMBOO ENGINE [V1]");
		system("cls");
		system("color 3E");
		Consol_Color(GREEN);
		std::cout << "\n\n		 BAMBOO ";
		Consol_Color(YELLOW);
		std::cout << "CONSOLENGINE\n\n";
		Consol_Color(GRAY);
		std::cout << "		[DATA CONTROL]   [D]\n";
		std::cout << "		[PIXCEL CONTROL] [P]\n";
		std::cout << "		[PAINT CONTROL]  [A]\n";
		std::cout << "		[EXIT]           [X]\n";
		button = getch();
		if (button == 'd')
		{
			DATA_MENU();
		}
		else if (button == 'p')
		{
			DRAW_MENU();
		}
		else if (button == 'a')
		{
			PAINT_MENU();
		}
		else if (button == 'x')
			break;
	}
}
void DATA_MENU(void)
{
	system("cls");
	Consol_Color(WHITE);
	HWND myconsole = GetConsoleWindow();
	RECT r;
	GetWindowRect(myconsole, &r);
	std::cout << "[FILE CREATE][C]\n";
	std::cout << "[FILE LOAD]  [L]\n";
	std::cout << "[FILE WRITE] [W]\n";
	std::cout << "[FILE ADDING][A]\n";
	std::cout << "[TEXT SPLITING][S]\n";
	std::cout << "[GOBACK MENU][x]\x1B[36m\n";
	char push = ' ';
	string file_name;
	string text;
	string L_text;
	while (true)
	{
		push = getch();
		Consol_Color(ORANGE);
		if (push == 'x')
		{
			break;
		}
		switch (push)
		{
		case 'c':
			std::cout << "	|FILE| CREAT\n";
			std::cout << "	|FILE| NAME : ";
			std::getline(cin, file_name);
			FileCreate(file_name);
			break;
		case 'l':
			std::cout << "	|FILE| LOAD\n";
			std::cout << "	|FILE| NAME : ";
			std::getline(cin, file_name);
			FileShow(file_name);
			break;
		case 'w':
			std::cout << "	|FILE| WRITING\n";
			std::cout << "	|FILE| NAME : ";
			std::getline(cin, file_name);
			std::cout << endl;
			std::cout << "	|FILE| INPUT TEXT : ";
			std::getline(cin, text);
			FileEdit(file_name, text);
			break;
		case 'a':
			std::cout << "	|FILE| ADDING\n";
			std::cout << "	|FILE| NAME : ";
			std::getline(cin, file_name);
			std::cout << endl;
			std::cout << "	|FILE| OUTPUT TEXT : ";
			std::getline(cin, text);
			FileAdd(file_name, text);
			break;
		case 's':
		{
			//std::getline(cin, file_name);
			std::cout << "	|FILE| SPLITING TEXT\n";
			std::cout << "	|FILE| NAME (put .txt) : ";
			std::getline(cin, file_name);
			std::cout << endl;
			std::cout << "	|FILE| Criterion word : ";
			char criterion = ' ';
			std::cin >> criterion;
			FileRead(file_name, L_text);
			char* target = (char*)malloc(1);
			int i = 0;
			char* text = (char*)malloc(L_text.size()+1);
			strcpy(text, L_text.c_str());
			cout << "[INPUT] : " <<text << endl;
			while (true)
			{
				Consol_Color(GREEN);
				Char_split(text, target, criterion);
				std::cout << "[FILE] SPLIT : " << target << " | REMAIN : " << text << endl;
				if (Char_size(text) == 0)
				{
					cout << "[FILE] : FINISHED SPLIT" << endl;
					break;
				}
			}
			pointer_cleaner(target);
			pointer_cleaner(text);
			cout << "[FILE] : FREE POINTERS" << endl;
			break;
		}			
		default:
			break;
		}
	}
}
void DRAW_MENU(void)
{
	system("cls"); //clear all screen
	char c = ' '; //variable for input button
	//get handle of console window
	HWND myconsole = GetConsoleWindow();
	RECT r;
	GetWindowRect(myconsole, &r);
	//making frame class for
	frame* f = (frame*)malloc(sizeof(frame)); 
	//To change coordinate system of screen. I made frame class which is used for changing y coordinate
	//The 0,0 point of Console Pixel fn is start from upper left corner
	//SO we have to change it left down corner
	//set the frame w as the width of console window
	f->w = r.right - r.left;
	f->h =(r.bottom - r.top); //same
	//for drawing line set the point vector2
	vector2 A = { 0,1 };
	vector2 B = { f->w-10, 1 };
	vector2 C = { f->w-10, f->h - 200};
	vector2 D = { 0, f->h- 200};
	//the origin point of frame
	f->o_x = 10;
	f->o_y = f->h;
	int cx = 0;
	int cy = 0;
	while (true)
	{
		getxy(cx,cy); //get position of cusor then
		clearScreen(0,0, cx, cy); //erasing the area from first text to current text
		gotoxy(0, 0); //removing cursor for writing the menu texts
		//drawing line for frame
		Consol_Line(WHITE_28, A, B, f);
		Consol_Line(WHITE_28, B, C, f);
		Consol_Line(WHITE_28, C, D, f);
		Consol_Line(WHITE_28, D, A, f);
		Consol_Color(TURQUOISE);
		//show draw menu
		std::cout << "- Creating point[P]" << endl;
		std::cout << "- Drawing line  [L]" << endl;
		std::cout << "- Drawing CIRCLE[C]" << endl;
		std::cout << "- Drawing SQUARE[S]" << endl;
		std::cout << "- ERASING       [E]" << endl;
		std::cout << "- Exit          [X]" << endl;
		//menu
		c= getch();
		if (c == 'x')
			break;
		Consol_Color(YELLOW);
		switch (c)
		{
		case 'p':
		{
			int x, y;
			std::cout << " writing x and y position : ";
			cin >> x >> y;
			vector2 temp_p = { x,y };
			Consol_Dot(GREEN_28, temp_p, f);
			std::cout << " YOUR DOT IS CREATED";
			break;
		}
		case 'l':
		{
			int x, y;
			int a, b;
			std::cout << " 1)first point : ";
			cin >> x >> y;
			vector2 temp_p1 = { x,y };
			std::cout << " 2)second point : ";
			cin >> a >> b;
			std::cout << temp_p1.y << endl;
			vector2 temp_p2 = { a,b };
			Consol_Line(GREEN_28, temp_p1, temp_p2, f);
			std::cout << " YOUR LINE IS CREATED";
			break;
		}
		case 'c':
		{
			int x, y, r;
			std::cout << "[circle] xy position then r: ";
			cin >> x >> y >> r;
			vector2 temp = { x,y };
			Consol_Circle(temp, r, f);
			break;
		}
		case 's':
		{
			int x, y, r;
			std::cout << "[square] xy position then r: ";
			cin >> x >> y >> r;
			vector2 temp = { x,y };
			Consol_Square(temp, r, f);
			break;
		}
		case 'e':
		{
			system("cls"); //for clear all screen
			break;
		}
		default:
			break;
		}
	}
	std::free(f); //frame is declared as malloc so it should be free;
}
void PAINT_MENU(void)
{
	system("cls"); //clear all screen
	char c = ' '; //variable for input button
	
	//get handle of console window
	HWND myconsole = GetConsoleWindow();
	RECT r;
	GetWindowRect(myconsole, &r);
	//making frame class for
	frame* f = (frame*)malloc(sizeof(frame));
	//set the frame w as the width of console window
	f->w = r.right - r.left;
	f->h = (r.bottom - r.top);
	//the origin point of frame
	f->o_x = 10;
	f->o_y = f->h;
	//---------circle
	bool exit = false;
	vector2 cir_p = {50, 50};
	//position variable
	int p_x = 0; 
	int p_y = 0; 
	int speed = 0;
	//file load variable
	string file_name;
	//menu text
	Consol_Color(GRAY);
	std::cout << "     [W]        \n";
	std::cout << "[A]< [S] >[D]    CLEAR PAPER[C]\n";
	std::cout << "[GOBACK MENU][x]\n\n";

	while (!exit)
	{
		Sleep(1); //for showing animation 
		clearScreen(0, 4, 0, 4); 
		gotoxy(0, 3);
		std::cout << cir_p.x << ", " << cir_p.y << endl;
		Consol_Circle(cir_p, 20, f);
		//----------
		p_x = 0;
		p_y = 0;
		speed = 4;
		c = getch();
		if (c == 'x')
			break;
		switch (c) //until now this only can get one key input.
		{
		case 'w':
		{
			if (!p_y)
				p_y = speed;
			break;
		}
		case 's':
		{
			if (!p_y)
				p_y = -speed;
			break;
		}
		case 'a':
		{
			if (!p_x)
				p_x = -speed;
			break;
		}
		case 'd':
		{
			if (!p_x)
				p_x = speed;
			break;
		}
		case 'c':
		{
			system("cls");
			break;
		}
		default:
			break;
		}
		cir_p.x += p_x;
		cir_p.y += p_y;
	}
	//destory the frame function which is get by the memory
	std::free(f);
}
//-----------------------------------Graphic function
void Consol_Line(int R, int G, int B, vector2 p1, vector2 p2, frame *taf)//RGB : color (255), vector, taf : target frame for drawing
	{
		//Get a console handle
		HWND myconsole = GetConsoleWindow();
		//Get a handle to device context
		HDC mydc = GetDC(myconsole);
		COLORREF COLOR = RGB(R, G, B);

		p1.y = taf->o_y - p1.y + 1; //to upside down the y position 
		p2.y = taf->o_y - p2.y + 1;
		p1.x += taf->o_x; //but not x
		p2.x += taf->o_x;

		int dis_x = abs(p1.x - p2.x);
		int dis_y = abs(p1.y - p2.y);

		if (dis_x != 0 && dis_y != 0) //when angle is 0 the line is horizontal or vertical
		{
			float slope = (float)(p2.y - p1.y) / (float)(p2.x - p1.x); //slope
			int s = min(p1.x, p2.x); //to calculate easily
			float y = 0;
			int endpoint = s + dis_x; 
			for (int i = s; i <= endpoint; i++)//from min x value to max x value
			{
				y = slope * (i - p1.x) + p1.y; 
				SetPixel(mydc, i, (int)y, COLOR);
			}
		}
		else if (dis_x != 0 && dis_y == 0) //Hori
		{
			int s = min(p1.x, p2.x);
			int endpoint = s + dis_x;
			for (int i = s; i <= endpoint; i++)//from min x value to max x value
			{
				SetPixel(mydc, i, p1.y, COLOR);
			}
		}
		else if (dis_x == 0 && dis_y != 0)//Verti
		{
			int s = min(p1.y, p2.y);
			int endpoint = s + dis_y; 
			for (int i = s; i <= endpoint; i++)//from min y value to max y value
			{
				SetPixel(mydc, p1.x, i, COLOR);
			}
		}
		else//both delta x y are zero is point
			SetPixel(mydc, p1.x, p1.y, COLOR);
		ReleaseDC(myconsole, mydc);
	}
void Consol_Dot(int R, int G, int B, vector2 p, frame *taf)
	{
		HWND myconsole = GetConsoleWindow();
		HDC mydc = GetDC(myconsole);
		COLORREF COLOR = RGB(R, G, B);
		p.y = taf->h - p.y;
		p.x += taf->o_x;
		SetPixel(mydc, p.x, p.y, COLOR);
		ReleaseDC(myconsole, mydc);
	}
void Consol_Circle(vector2 p, int r, frame *taf) //drawing from center
	{
		HWND myconsole = GetConsoleWindow();
		HDC mydc = GetDC(myconsole);
		COLORREF COLOR = RGB(0,255,0);

		for (int i = -r; i <= r; i++)
		{
			for (int j = - r; j <= r; j++)
			{
				double d = (i*i*3.14+j*j*3.14) / (r*r);
				if(d <=1) //if you change this condition, you can make a line circle
				{
					SetPixel(mydc, i + p.x + taf->o_x, taf->h - (j + p.y), COLOR);
				}
			}
		}
		ReleaseDC(myconsole, mydc);
	}
void Consol_Square(vector2 p, int r, frame *taf)//drawing from center
	{
		HWND myconsole = GetConsoleWindow();
		HDC mydc = GetDC(myconsole);
		COLORREF COLOR = RGB(0, 255, 0);
		//x^2+y^2 = r^2 -> y^2 = r^2-x^2 -> y = sqrt(r^2-x^2) and -
		for (int i = p.x - r; i <= p.x + r; i++)
		{
			for (int j = p.y - r; j <= p.y + r; j++)
			{
				SetPixel(mydc, i + taf->o_x, taf->h - j, COLOR);
			}
		}
		ReleaseDC(myconsole, mydc);
	}

