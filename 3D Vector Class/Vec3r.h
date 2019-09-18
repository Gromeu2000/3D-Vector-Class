#pragma once
#include <iostream>

class Vec3d {

public:
	int x;
	int y;
	int z;

	Vec3d() { x = 0; y = 0; z = 0; };
	Vec3d(int x1, int y1, int z1) { x=x1; y=y1; z=z1; };
	Vec3d(const Vec3d &vector);

	Vec3d operator+(const Vec3d &vector)
	{
		return Vec3d(x + vector.x, y + vector.y, z + vector.z);
	}

	Vec3d operator-(const Vec3d &vector)
	{
		return Vec3d(x - vector.x, y - vector.y, z - vector.z);
	}

	Vec3d operator+=(const Vec3d &vector)
	{
		return Vec3d(x += vector.x, y += vector.y, z += vector.z);
	}

	Vec3d operator-=(const Vec3d &vector)
	{
		return Vec3d(x -= vector.x, y -= vector.y, z -= vector.z);
	}

	Vec3d operator=(const Vec3d &vector) 
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	bool operator==(const Vec3d &vector) 
	{

		if (x == vector.x && y == vector.y && z == vector.z) {

			return true;
		}
		else {

			return false;
		}
	}

	Vec3d operator/(int value)
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}


	void Zero() {

		x = 0;
		y = 0;
		z = 0;
	}

	Vec3d normalization()
	{
		
		*this / v_magnitude();
		return *this;
	}

	bool is_Zero() const{ 

		return x == 0 && y == 0 && z == 0;
	}

	int v_square()
	{
		return x * x + y * y + z * z;
	}

	int v_magnitude()
	{
		return sqrt(v_square());
	}

	int distance_to(const Vec3d &vector)
	{
		Vec3d dist = *this - vector;
		return dist.v_magnitude;
	}

	void print()
	{
		std::cout << x << " " << y << " " << z << std::endl;
	}
};