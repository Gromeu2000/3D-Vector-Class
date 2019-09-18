#include <iostream>
#include "Vec3r.h"

int main() {

	Vec3d u = Vec3d(1, 1, 1);
	Vec3d v = Vec3d(3, 7, 10);

	Vec3d w = u + v;
	Vec3d a = u - v;
	Vec3d b = u += v;
	Vec3d c = u -= v;

	w.print();
	a.print();
	b.print();
	c.print();

	system("pause");
	return 0;
}