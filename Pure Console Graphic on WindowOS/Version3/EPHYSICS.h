#pragma once
#define CONCAVE 0
#define CONVEX 1

//TYPE : CONCAVE or CONVEX
float* LENS(float focal, float object, const char TYPE)
{
	switch (TYPE)
	{
	case CONCAVE:
	{
		focal = -focal;
	}
	case CONVEX:
	{
		
	}
	default:
	
	}
}