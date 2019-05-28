#pragma once
#include "W_consol.h"

//----------Dynamic char list controller--------------------------------
void Char_split(char* &intext , char* &outtext, char split_cod) 
// we will change the address directly and deep copying of array
//char* & is the reference of pointer (just give another name of the same pointer)
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
			pointer_cleaner(temp); //the temp value is std::free in this function but other intext and outtext should be std::free, after using these at outside of fn 
		}
	}
} //reference of pointer
int Char_size(char* &text)
//Call by Reference
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
/*				_temp = 0;
				unsigned int power = 1;
				for (int k = j-1; k >= j - n_continue; k--) //9. Cus j=3, if(k =2; k>3-2; k--) power = 1; 
				{
					_temp += (bit_eq[k]-0x30)*power ; //10. _temp = 8; 12. _temp = 8 + 4*10;
					power = power * 10; //11. power = 10; 13. power = 100;
				}
				cout << _temp << endl;*/
void MATH_TEXT_ANALAZING(char* &text)
{
	int _size = Char_size(text) ;
	char* bit_eq = (char*)malloc((_size)+1);
	int i = 0;
	int n = 0;
	int _n_space = 0;
	//ex) +4  5 * 6( 3 -2)
	while (i < _size)
	{	
		if (text[i] == ' ');
		else
		{
			bit_eq[n] = text[i];
			std::cout<<bit_eq[n];
			n++;
		}
		i++;
	}
	std::cout <<"<- resizing number : " <<n << "\n";
	bit_eq[n] = 0x00; //for last ending
	n++;

	unsigned char n_continue = 0;
	int _temp = 0;
	Math_sym* new_bit_eq = (Math_sym*)malloc(sizeof(Math_sym)*n);
	//ex) +45*6(3-2)
	for (int j = 0; j < n; j++)
	{		
		if (bit_eq[j] < 0x30 || bit_eq[j] > 0x39) // 1. '+' is text |3. '4' is number |5. '5'is number | 7. '*' is text
		{
			if (n_continue >= 1) // 2. n is = 0  | 8. n is = 2
			{

			}
			n_continue = 0;
		}
		else
			n_continue++; // 4. n is 1| 6. n is 2
	}
	std::free(bit_eq);
}
void pointer_cleaner(char* &ptr)
//Call-by-reference
//to clean up other elemental of array
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
	std::free((void*)ptr[0]);
} //need to edit
//-----------ConSole UI function----------------------------------------
void main_menu(void)
{
	char button;
	while (true)
	{
		system("color 07");
		Console_Window(55, 30);
		SetConsoleTitle("BAMBOO ENGINE[v1]");
		Console_Color(GREEN);
		std::cout << "\n\n		 BAMBOO ";
		Console_Color(YELLOW);
		std::cout << "CONSOLENGINE\n\n";
		Console_Color(GRAY);
		std::cout << "		[DATA CONTROL]   [D]\n";
		std::cout << "		[PIXCEL CONTROL] [P]\n";
		std::cout << "		[PAINT CONTROL]  [W]\n";
		std::cout << "		[THREAD]         [T]\n";
		std::cout << "		[Mathmatica]     [M]\n";
		std::cout << "		[A* Algorithm]   [A]\n";
		std::cout << "		[Machine learn]  [C]\n";
		std::cout << "		[Physics]        [K]\n";
		std::cout << "		[EXIT]           [X]\n";
		button = getch();
		if (button == 'd')
		{
			DATA_MENU();
		}
		else if (button == 'm')
		{
			MATH_MENU();
		}
		else if (button == 'a')
		{
			A_start_MENU();
		}
		else if (button == 'c')
		{
			Machine_MENU();
		}
		else if (button == 'k')
		{
			Physics_MENU();
		}
		else if (button == 'x')
			break;
	}
}
void DATA_MENU(void)
{
	system("cls");
	Console_Color(WHITE);
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
		Console_Color(ORANGE);
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
			std::cout << "[INPUT] : " <<text << endl;
			while (true)
			{
				Console_Color(GREEN);
				Char_split(text, target, criterion);
				std::cout << "[FILE] SPLIT : " << target << " | REMAIN : " << text << endl;
				if (Char_size(text) == 0)
				{
					std::cout << "[FILE] : FINISHED SPLIT" << endl;
					break;
				}
			}
			pointer_cleaner(target);
			pointer_cleaner(text);
			std::cout << "[FILE] : std::free POINTERS" << endl;
			break;
		}			
		default:
			break;
		}
	}
}
void MATH_MENU(void)//using char spliting we distingush the number and variable and symbol
{
	Console_Window(75, 15);
	SetConsoleTitle("MATHTOPIA[v1]");
	system("COLOR 80");
	string equation;
	std::cout << "[MATH] Typing your equation"<<endl;
	std::getline(cin, equation);
	char* text = (char*)malloc(equation.size() + 1);
	strcpy(text, equation.c_str());
	MATH_TEXT_ANALAZING(text);
	getch();
}
void A_start_MENU(void)
{
	Console_Window(80, 100);
	SetConsoleTitle("A* ALGORITHM[v1]");
	system("COLOR 00");
	bool stop = false;
	while (!stop)
	{
		system("cls");
		//---------------------DRAWING MAP-----------
		for (unsigned int i = 2; i < map[1]; i++)
		{
			if (map[i] == 1)
			{
				gotoxy(i % map[0], i / map[0]);
				Console_Color(WHITE);
				std::cout << "\u001b[47;1mH";
			}
			std::cout << "\u001b[0m";
		}
		//-------------------------------------------
		int sx, sy, ex, ey;
		//-------------------------STARTING POINT
		gotoxy(0, 25);
		Console_Color(WHITE);
		std::cout << "[A*] Typing your start point x, y" << endl;
		gotoxy(0, 26);
		cin >> sx >> sy;
		int start = Xytoarray(sx, sy);
		Console_Color(YELLOW);
		gotoxy(sx, sy);
		std::cout << "S";

		//-------------------------ENDING POINT	
		gotoxy(0, 27);
		Console_Color(WHITE);
		std::cout << "[A*] Typing your end point x, y" << endl;
		gotoxy(0, 28);
		cin >> ex >> ey;
		int goal = Xytoarray(ex, ey);
		Console_Color(YELLOW);
		gotoxy(ex, ey);
		std::cout << "G";
		gotoxy(0, 29);
		Console_Color(WHITE);
		std::cout << "- Start A* [S]\n- STOP [X]  RESET [ANY KEY]";
		char c = _getch();
		switch (c)
		{
		case 's':
			A_algorithm(start, goal);
			Console_Color(YELLOW);
			gotoxy(sx, sy);
			std::cout << "S";
			gotoxy(ex, ey);
			std::cout << "G";
			_getch();
			break;
		case 'x':
			stop = true;
			break;
		default:
			break;
		}
	}
}
void Machine_MENU(void)
{
	Console_Window(50, 45);
	frame f(30, 20, 0, 0);
	SetConsoleTitle("MACHINE LEARNING[v1]");
	system("color 70");
	system("cls");
	std::cout << "y = cos|x [0,30] so y [-pi,pi]";
	vector2 p1 = { 0, 0 };
	vector2 p2 = { 0, 305 };
	vector2 p3 = { 250,0 };
	for (int k = 0; k < p2.y; k++)
	{
		vector2 temp_p1 = { 0,k };
		Console_Dot(0, 0, 0, temp_p1, f);
	}
	for (int t = 0; t < p3.x; t++)
	{
		vector2 temp_p2 = { t,0 };
		Console_Dot(0, 0, 0, temp_p2, f);
	}
	//----------------------------------
	int draw_y = 60;
	for (float j = 0; j < 20; j += 0.01f)
	{
		vector2 p = { j * 20, cos(j) * 50 + draw_y }; //making unit of x as 20
		Console_Dot(0, 0, 0, p, f);
	}
	_getch();
	std::cout << "PREPARING ";

	//INITIAL
	M_Node* f_node = (M_Node*)malloc(sizeof(M_Node) *20);
	unsigned int* f_weight = (unsigned int*)malloc(sizeof(unsigned int) * 20);
	clock_t start, end, smallest;
	smallest = 999999;

	//--------------------------
	int n = 0;
	while(n<20)
	{
		unsigned int* temp_weight = (unsigned int*)malloc(sizeof(unsigned int) * 20);
		M_Node* temp_node = (M_Node*)malloc(sizeof(M_Node) * 20);
		start = clock();
		srand(time(NULL));//AFTER 1970 1m 1d 0h
		float ran_d;
		for (int i = 0; i < 20; i++)
		{
			temp_node[i].in_put = i;
			//-----Giving Weight
			temp_node[i].formula = rand() %10 + 20;
			//random choose
			float actual_val = cos(i);
			while (temp_node[i].out_put != actual_val)
			{
				ran_d = rand() % (int)temp_node[i].formula;
				temp_node[i].out_put = cos(ran_d);
				gotoxy(0, 1);
				cout <<"<"<< ran_d <<">[" << i << "] ACTUAL : " << cos(i) << "  SUGGEST : " << temp_node[i].out_put;
				Sleep(2);
			}
			
		}
		end = clock();
		clock_t time = end - start;
		gotoxy(0, 2+n);
		cout << "[" << n << "] TAKING TIME :  " << time;

		if (time < smallest)
		{
			memcpy(f_node, temp_node,  sizeof(M_Node)*20);
			smallest = time;
		}

		for (int i = 0; i < 20; i++)
		{
			temp_weight[i] = NULL;
		}
		std::free(temp_weight);

		for (int i = 0; i < 20; i++)
		{
			temp_node[i].in_put= NULL;
			temp_node[i].out_put = NULL;
			temp_node[i].formula = NULL;
		}
		std::free(temp_node);
		n++;
	}

	gotoxy(0, 22);
	for (int u = 0; u < 20; u++)
	{
		vector2 p_c_1 = { u * 20 , f_node[u].out_put * 50 + draw_y };
		vector2 p_c_2 = { (u + 1) * 20 , f_node[u + 1].out_put * 50 + draw_y };
		if (u % 4 == 0)
			cout << endl;
		cout << "["<< 0 << " ~ "<< f_node[u].formula -20 <<"]";

		Console_draw_line(RED_28, p_c_1, p_c_2, f);
		Sleep(100);
	}
	std::cout << "<END>" ;
	std::free(f_node);
	std::free(f_weight);
	_getch();
}
void Physics_MENU(void)
{
	Console_Window(80, 35);
	frame f(600,300,10,380);

	SetConsoleTitle("Physics[v1]");
	//-----initial values----------
	Input_value useInput;
	vector2 A = { 0, 0 };
	vector2 B = { f.w, 0 };
	vector2 C = { f.w, f.h};
	vector2 D = { 0, f.h};
	bool exit = 0;
	gotoxy(0, 0);
	system("color 07");
	while (!exit)
	{
		system("cls");
		Console_draw_line(200, 200, 200, A, B, f);
		Console_draw_line(200, 200, 200, B, C, f);
		Console_draw_line(200, 200, 200, C, D, f);
		Console_draw_line(200, 200, 200, D, A, f);
		std::cout << "  Equivalent Speed[E]\n  Circular motion [c] EXIT [X]\n";

		switch (_getch())
		{
		case'e':
		{
			//-------object value-------------------------------------------------------
			float x, y, x_0, y_0, ax, ay, Fx, Fy, vx_0, vy_0, vx, vy, mass, impulse;
			float g = -9.8f;
			float t = 0;
			float impulse_t = 1;
			//---------------------------------------------------------------------------
			gotoxy(0, 0);
			std::cout << "[1]  Enter initial pos of x and y : ";
			cin >> x_0 >> y_0;
			std::cout << "[1]  Enter initial speed of x and y : ";
			cin >> vx_0 >> vy_0;
			std::cout << "[1]  Enter mass of the object : ";
			cin >> mass;
			std::cout << "[1]  Enter Force of x and y : ";
			cin >> Fx >> Fy;
			std::cout << endl;
			//------------object setting-------------------------------------------------
			vector2f pos = { x_0, y_0 };
			vector2f Force = { Fx, Fy };
			vector2f Accelration = { 0,0 };
			vector2f velocity = { vx_0,vy_0 };
			//------------------------OBJECT----------------------------------------------
			Object obj1;
			obj1.id = &obj1;
			obj1.a = Accelration;
			obj1.v = velocity;
			obj1.F = Force;
			obj1.pos.x = (int)pos.x;
			obj1.pos.y = (int)pos.y;
			obj1.mass = mass;
			obj1.R = 10;
			obj1.E = 0;
			obj1.k = 0;
			//------------------------------------------------
			std::cout << "[2] Enter initial pos of x and y : ";
			cin >> x_0 >> y_0;
			std::cout << "[2]  Enter initial speed of x and y : ";
			cin >> vx_0 >> vy_0;
			std::cout << "[2]  Enter mass of the object : ";
			cin >> mass;
			std::cout << "[2]  Enter Force of x and y : ";
			cin >> Fx >> Fy;
			std::cout << endl;
			std::cout << "[3] Enter time of impulse : ";
			cin >> impulse_t;
			//------------object setting-------------------------------------------------
			pos = { x_0, y_0 };
			Force = { Fx, Fy };
			Accelration = { 0,0 };
			velocity = { vx_0,vy_0 };
			//------------------------OBJECT----------------------------------------------
			Object obj2;
			obj2.id = &obj2;
			obj2.a = Accelration;
			obj2.v = velocity;
			obj2.F = Force;
			obj2.pos.x = (int)pos.x;
			obj2.pos.y = (int)pos.y;
			obj2.mass = mass;
			obj2.R = 10;
			obj2.E = 0;
			obj2.k = 0;
			//-----------------------------------
			vector<Object> obj_list1, obj_list2;
			vector<Object*> col_list;
			obj_list1.push_back(obj1);
			obj_list2.push_back(obj2);
			col_list.push_back(&obj1);
			col_list.push_back(&obj2);
			//-------------
			clock_t start = clock();
			int col_num = 0;
			//---------------------------------------------------------------------------
			while (true)
			{
				Simul(obj1, 0.2);
				obj_list1.push_back(obj1);
				Simul(obj2, 0.2);
				obj_list2.push_back(obj2);
				if (Simul_col(obj1, col_list, impulse, impulse_t))
				{
					gotoxy(0, 15);
					col_num++;
					cout << "HOW MANY COLLISION [ " << col_num << " ]" << endl;
					cout << "change A : " << obj1.v.x << " | " << obj1.v.y << endl;
					cout << "change B : " << obj2.v.x << " | " << obj2.v.y << endl;
					cout << "Magnitude IMPULSE : " << impulse << endl;
				}
				t += 0.04;
				if (t > 5)
					break;
			}
			//-------------Drawing Part----------------------------------
			thread t1(&drawing, std::ref(obj_list1), 0, 255, 0, obj1.R, std::ref(f));
			thread t2(&drawing, std::ref(obj_list2), 255, 0, 0, obj2.R, std::ref(f));
			t1.join();
			t2.join();
			//-------------------------------------------------------------
			clock_t end = clock();
			cout << " Taking time of Simulation : " << (float)(end - start) << endl;
			_getch();
			break;
		}
		case 'c':
		{
			float x, y, x_0, y_0, ax, ay, Fx, Fy, vx_0, vy_0, vx, vy, mass, impulse;
			float g = -9.8f;
			float t = 0;
			//---------------------------------------------------------------------------
			gotoxy(0, 0);
			std::cout << "[1]  Enter initial pos of x and y : ";
			cin >> x_0 >> y_0;
			std::cout << "[1]  Enter initial speed of x and y : ";
			cin >> vx_0 >> vy_0;
			std::cout << "[1]  Enter mass of the object : ";
			cin >> mass;
			std::cout << "[1]  Enter Force of x and y : ";
			cin >> Fx >> Fy;
			std::cout << endl;
			//------------object setting-------------------------------------------------
			vector2f pos = { x_0, y_0 };
			vector2f Force = { Fx, Fy };
			vector2f Accelration = { 0,0 };
			vector2f velocity = { vx_0,vy_0 };
			//------------------------OBJECT----------------------------------------------
			Object obj1;
			obj1.id = &obj1;
			obj1.a = Accelration;
			obj1.v = velocity;
			obj1.F = Force;
			obj1.pos.x = (int)pos.x;
			obj1.pos.y = (int)pos.y;
			obj1.mass = mass;
			obj1.R = 5;
			obj1.E = 0;
			obj1.k = 0;
			//----------------------
			vector<Object> list;
			float theta = 0;
			while (t < 100)
			{
				if (theta < 6.328)
				{
					theta += 0.01f;
				}
				else
					theta = 0;
				Simul_Circle_Motion(obj1, 10, t, theta);
				list.push_back(obj1);
				t += 0.2;
			}
			thread t1(&drawing, std::ref(list), 0, 255, 0, obj1.R, std::ref(f));
			t1.join();
			getch();
			break;
		}
		case 'x':
			exit = true;
			break;
		default:
			break;
		}
	}
}
//-----------------------------------Graphic---------------------------------------
void drawing(vector<Object>& const drawing_list, int R, int G, int B, float radius, frame & const f)
{
	for (int k = 0; k < drawing_list.size(); k++)
	{
		//gotoxy(0, 10);
		//cout <<"A [" << drawing_list[k].a.x << "|"<< drawing_list[k].a.y <<"] pos [" <<drawing_list[k].pos.x << " | " << drawing_list[k].pos.y << "] | V [" <<drawing_list[k].v.x << " | " << drawing_list[k].v.y << "]";
		vector2 pos;
		pos.x = (int)drawing_list[k].pos.x;
		pos.y = (int)drawing_list[k].pos.y;
		Console_Circle(R, G, B, pos, radius, f);
		Sleep(10);
		Console_Circle(0, 0, 0, pos, radius, f);
	}
	for (int k = 0; k < drawing_list.size(); k += 2)
	{
		vector2 pos;
		pos.x = (int)drawing_list[k].pos.x;
		pos.y = (int)drawing_list[k].pos.y;
		Console_Circle(R, G, B, pos, 2, f);
	}
}