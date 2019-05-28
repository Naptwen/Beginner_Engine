#pragma once
#include <iostream>
#include <vector>

using namespace std;

class vector2f
{
public:
	float x;
	float y;
	void operator = (float i[2]) //for easy way to declare x,y
	{
		this->x = i[0];
		this->y = i[1];
	}
};
class Object
{
public:
	Object* id;
	vector2f pos;//center
	vector2f F;
	vector2f a;
	vector2f v;
	vector2f P;
	float E;
	float mass;
	float R;
	float k;
};
//----------------------------------------------------------------------
void Simul(Object &obj, float t);
bool Simul_col(Object & const obj, vector<Object*> & const list, float &impulse, float &t);
bool Simul_Circle_Motion(Object &obj, float R, float theta, float t);