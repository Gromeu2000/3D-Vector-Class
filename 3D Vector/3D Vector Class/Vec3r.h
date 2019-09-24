#ifndef _HEADER_H

#include <stdio.h>
#include <locale.h>
#include <iostream>
using namespace std;

template <class T>


class vec3
{
public:

	T x, y, z;

	vec3(T x1, T y1, T z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}

	// NORMALIZE
	void normalize()
	{
		float module;
		module = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		x = x / module;
		y = y / module;
		z = z / module;
	}

	// CHANGE THE PARAMETERS OF THE VECTOR TO 0
	void zero()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	// CHECK IF VECTOR IS 0
	bool is_zero()
	{
		if (x == 0 && y == 0 && z == 0)
		{
			return true;
		}

		return false;
	}

	// CALCULATE DISTANCE
	float distance_to(vec3 v1)
	{
		float result;
		result = sqrt(pow((v1.x - x), 2) + pow((v1.y - y), 2) + pow((v1.z - z), 2));
		return result;
	}

	// ADDITION
	vec3 operator+(vec3 <float> v)
	{
		vec3 <float> res(0, 0, 0);
		res.x = this->x + v.x;
		res.y = this->y + v.y;
		res.z = this->z + v.z;
		return res;
	}

	//SUBSTRACTION
	vec3 operator-(vec3 <float> v)
	{
		vec3 <float> res(0, 0, 0);
		res.x = this->x - v.x;
		res.y = this->y - v.y;
		res.z = this->z - v.z;
		return res;
	}

	vec3 operator+=(vec3 <float> v)
	{
		vec3 <float> res(0, 0, 0);
		res.x = this->x += v.x;
		res.y = this->y += v.y;
		res.z = this->z += v.z;
		return res;
	}

	vec3 operator-=(vec3 <float> v)
	{
		vec3 <float> res(0, 0, 0);
		res.x = this->x -= -v.x;
		res.y = this->y -= -v.y;
		res.z = this->z -= -v.z;
		return res;
	}

	vec3 operator==(vec3 <T>v1) {
		{
			if (this->x == v1->x && this->y == v1->y && this->z == v1->z)
			{
				return true;
			}
			return false;
		}
	}
};

#endif //_HEADER_H