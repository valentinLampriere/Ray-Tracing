#pragma once

#include "lib/vector.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

class Box {
public:
	Vector3 coord1, coord2;
	Box* childBox1;
	Box* childBox2;
	vector<Sphere> spheres;

	Box();
	Box(Vector3 c1, Vector3 c2);
	static Box Union(const Box b1, const Box b2) {
		Vector3 min = Vector3::min(b1.coord1, b2.coord1);
		Vector3 max = Vector3::max(b1.coord2, b2.coord2);
		return Box(min, max);
	}
	Box Union(Box b);
	void split(Box* b1, Box* b2);
	bool rayHit(Ray r, float* t);
	void settingSpheres(std::vector<Sphere> allSpheres);
	bool hasChildren();
	void print();
};