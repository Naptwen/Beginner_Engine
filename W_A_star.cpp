#include "W_A_star.h"



void Sorting(vector<int> &temp_open_list, vector<float[5]> &temp_GHF)
{
	if (temp_open_list.size() > 0)
	{
		for (int j = 0; j < temp_open_list.size(); j++)
		{
			for (int i = j; i < temp_open_list.size(); i++)
			{
				if (temp_GHF[temp_open_list[i]][2] > temp_GHF[temp_open_list[j]][2])
				{
					int temp = temp_open_list[j];
					temp_open_list[j] = temp_open_list[i];
					temp_open_list[i] = temp;
				}
			}
		}
	}
}
bool IsVectorHAS(vector<int> &list, int target)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == target)
			return 1;
	}
	return 0;
}
void Arraytoxy(int &array_num, int &x, int &y)
{
	x = array_num % map[0];
	y = array_num / map[0];
}
int Xytoarray(int x, int y)
{
	return x + map[0] * y;
}
void F_G_H_CAL(int num, int map[], vector<int> &temp_open_list, vector<int> &temp_close_list, vector<float[5]> &temp_GHF, int &sp, int &ep)
{
	if (num < map[1] && num > map[0] && map[num] != 1 && !IsVectorHAS(temp_open_list, num) && !IsVectorHAS(temp_close_list, num))
	{
		temp_GHF[num][0] = temp_GHF[sp][0] + 1;
		int x, y, e_x, e_y;
		Arraytoxy(num, x, y);
		Arraytoxy(ep, e_x, e_y);
		int dis = sqrt((x - e_x) *(x - e_x) + (y - e_y) * (y - e_y));

		temp_GHF[num][1] = dis;
		temp_GHF[num][2] = temp_GHF[num][0] + temp_GHF[num][1];
		temp_GHF[num][3] = sp;
		temp_GHF[num][4] = num;
		temp_open_list.push_back(num);
		gotoxy(x, y);
		std::cout << "\u001b[37;1m+";
		//DATA_PRINTER();
		//printf("ADD <%d,%d> [%d]", x,y, (int)temp_GHF[num][2]);
	}
}
void A_algorithm(int sp, int ep)
{
	int s_p = sp;
	std::vector<float[5]> temp_GHF(map[1]);
	std::vector<int> temp_open_list;
	std::vector<int> temp_close_list;

	int jx, jy;
	Arraytoxy(sp, jx, jy);
	int ex, ey;
	Arraytoxy(ep, ex, ey);

	temp_GHF[sp][0] = 0;
	while (sp != ep)
	{
		int LEFT = sp - 1;
		int RIGHT = sp + 1;
		int UP = sp - map[0];
		int DOWN = sp + map[0];

		F_G_H_CAL(LEFT, map, temp_open_list, temp_close_list, temp_GHF, sp, ep);
		F_G_H_CAL(RIGHT, map, temp_open_list, temp_close_list, temp_GHF, sp, ep);
		F_G_H_CAL(UP, map, temp_open_list, temp_close_list, temp_GHF, sp, ep);
		F_G_H_CAL(DOWN, map, temp_open_list, temp_close_list, temp_GHF, sp, ep);
		Sleep(1);

		Sorting(temp_open_list, temp_GHF);

		if (temp_open_list.size() != 0)
		{
			sp = temp_open_list.back();
			int a, b;
			Arraytoxy(sp, a, b);
			gotoxy(a, b);
			std::cout << "\x1B[32mx";
			temp_open_list.pop_back();
			temp_close_list.push_back(sp);
		}
		else
		{
			break;
		}
		Sleep(1);
	}
	int back = ep;
	while (back != s_p)
	{
		int px, py;
		Arraytoxy(back, px, py);
		gotoxy(px, py);
		std::cout << "\u001b[35;1mo";
		back = temp_GHF[back][3];
		Sleep(20);
	}
}