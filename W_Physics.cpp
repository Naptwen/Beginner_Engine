#include "W_Physics.h"

void Simul(Object &obj, float t)
{
	obj.a.x = obj.F.x / obj.mass;
	obj.a.y = obj.F.y / obj.mass;
	obj.v.x = obj.v.x + obj.a.x * t;
	obj.v.y = obj.v.y + obj.a.y * t;
	obj.P.x = obj.mass * obj.v.x;
	obj.P.y = obj.mass * obj.v.y;
	float vector_V = sqrtf(obj.v.x*obj.v.x + obj.v.y*obj.v.y);
	obj.E = 0.5*obj.mass * vector_V * vector_V;
	obj.pos.x += obj.v.x * t;
	obj.pos.y += obj.v.y * t;
}
bool Simul_col(Object & const obj, vector<Object*> & const list, float &impulse, float &t)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (obj.id != list[i]->id)
		{
			float dis = sqrtf((obj.pos.x - list[i]->pos.x) * (obj.pos.x - list[i]->pos.x) + (obj.pos.y - list[i]->pos.y) * (obj.pos.y - list[i]->pos.y));
			float mindis = obj.R + list[i]->R;
			if (dis <= mindis)
			{
				//gotoxy(0, 10);
				//cout << "[!] Collision between [" << obj.id << "][" << list[i]->id << "]" << endl;

				//Change velocity
				float temp_obj_vx = obj.v.x;
				float temp_obj_vy = obj.v.y;
				float temp_trg_vx = list[i]->v.x;
				float temp_trg_vy = list[i]->v.y;
				obj.v.x = (obj.mass - list[i]->mass) / (obj.mass + list[i]->mass) * temp_obj_vx
					+ (2 * list[i]->mass / (obj.mass + list[i]->mass))*temp_trg_vx;

				obj.v.y = (obj.mass - list[i]->mass) / (obj.mass + list[i]->mass) * temp_obj_vy
					+ (2 * list[i]->mass / (obj.mass + list[i]->mass))*temp_trg_vy;

				list[i]->v.x = (list[i]->mass - obj.mass) / (obj.mass + list[i]->mass) * temp_trg_vx
					+ (2 * obj.mass / (obj.mass + list[i]->mass))*temp_obj_vx;

				list[i]->v.y = (list[i]->mass - obj.mass) / (obj.mass + list[i]->mass) * temp_trg_vy
					+ (2 * obj.mass / (obj.mass + list[i]->mass))*temp_obj_vy;

				//find new momentum
				float obj_Px = obj.mass * obj.v.x;
				float obj_Py = obj.mass * obj.v.y;
				float list_Px = list[i]->mass * list[i]->v.x;
				float list_Py = list[i]->mass * list[i]->v.y;

				//find the change of momentums
				float delta_obj_Px = obj_Px - obj.P.x;
				float delta_obj_Py = obj_Py - obj.P.y;
				float delta_list_Px = list_Px - list[i]->P.x;
				float delta_list_Py = list_Py - list[i]->P.y;

				//find impulse
				impulse = sqrt((delta_obj_Px) * (delta_obj_Px)+(delta_obj_Py) * (delta_obj_Py));
				if (t != 0)
				{
					cout << obj.F.x << "+" << delta_obj_Px / t;
					cout << list[i]->F.x << "+" << delta_list_Px / t;
					obj.F.x += delta_obj_Px / t;
					obj.F.y += delta_obj_Py / t;
					list[i]->F.x += delta_list_Px / t;
					list[i]->F.y += delta_list_Py / t;
					
				}
				//finally change the momentum (Actually this is not necessay)
				/*obj.P.x = obj_Px;
				obj.P.y = obj_Py;
				list[i]->P.x = list_Px;
				list[i]->P.y = list_Py;*/
				return true;
			}
		}

	}
	return false;
}
bool Simul_Circle_Motion(Object &obj, float R,float theta ,float t)
{
	obj.v.x = sqrt(10*R / obj.mass) * -cos(theta);
	obj.v.y = sqrt(10*R / obj.mass) * sin(theta);
	obj.pos.x = R * sin(theta) + obj.pos.x;
	obj.pos.y = R * cos(theta) + obj.pos.y;
	return true;
}