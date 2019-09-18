#pragma once

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
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	Vec3d operator-=(const Vec3d &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	Vec3d operator=(const Vec3d &vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	Vec3d operator==(const Vec3d &vector) {


	}
};